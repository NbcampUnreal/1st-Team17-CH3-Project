#include "Player/NXPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Player/NXPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/NXDoor.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "WeaponBase.h"
#include "Animation/NXCharacterAnimInstance.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Item/NXSpeedItem.h"



ANXPlayerCharacter::ANXPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArmComp->SetupAttachment(RootComponent);
    SpringArmComp->TargetArmLength = 300.0f;
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    CameraComp->bUsePawnControlRotation = false;


    NormalSpeed = 700.0f;
    SprintSpeedMultiplier = 1.7f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;


    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    
   
    

    MaxHealth = 100;
    Strength = 15;
    LevelDuration = 30000.0f;
    CurrentLevelIndex = 0;
    MaxLevels = 4;

    Defense = 1;
    bIsSitting = false;
    bIsDead = false;
    bHasKey = false;
   
   
}

void ANXPlayerCharacter::PickupKey()
{
    bHasKey = true;

    this->Tags.Add(FName("HasKey"));


    UE_LOG(LogTemp, Warning, TEXT("Key picked up!"));
}

void ANXPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    MaxHealth = Health;

    
   
    CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(Weaponclass);
   

    CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("hand_rSocket")));

    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &ANXPlayerCharacter::UpdateHUD,
        0.1f,
        true
    );
    GetWorldTimerManager().SetTimer(
        LevelTimerHandle,
        this,
        &ANXPlayerCharacter::OnLevelTimeUp,
        LevelDuration,
        false
    );
    UpdateHUD();

    UNXCharacterAnimInstance* AnimInstance = Cast<UNXCharacterAnimInstance>(GetMesh()->GetAnimInstance());
    if (IsValid(AnimInstance) == true)
    {
        AnimInstance->OnCheckHit.AddDynamic(this, &ThisClass::OnCheckHit);
    }

}

void ANXPlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);


    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        //공격(Attack) 바인딩?
        EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::InputAttack);
        EnhancedInput->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::InputFire);
        EnhancedInput->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::InputReload);

        if (ANXPlayerController* PlayerController = Cast<ANXPlayerController>(GetController()))
        {
            
            if (PlayerController->MoveAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANXPlayerCharacter::Move
                );
            }
            if (PlayerController->JumpAction)
            {

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANXPlayerCharacter::StartJump
                );


                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &ANXPlayerCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {

                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANXPlayerCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {

                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANXPlayerCharacter::StartSprint
                );

                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &ANXPlayerCharacter::StopSprint
                );
            }

            if (PlayerController->InteractAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->InteractAction,
                    ETriggerEvent::Completed,
                    this,
                    &ANXPlayerCharacter::Interact
                );
            }
            //Idle에서 Crouch(Sit)
            if (PlayerController->CrouchAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->CrouchAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANXPlayerCharacter::BeginCrouch
                );
            }
            //앉기에서 Idle(Stand)
            if (PlayerController->UnCrouchAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->UnCrouchAction,
                    ETriggerEvent::Triggered,
                    this,
                    &ANXPlayerCharacter::EndCrouch
                );
            }
            



        }
    }
}

void ANXPlayerCharacter::OnCheckHit()
{
    TArray<FOverlapResult> OverlapResults; // 충돌 감지 후 감지된 액터들을 담아 놓을 배열
    FCollisionQueryParams CollisionQueryParams(NAME_None, false, this); // 충돌 감지에 필요한 변수 선언
    bool bResult = GetWorld()->OverlapMultiByChannel(
        OverlapResults, GetActorLocation(), FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(300.f), CollisionQueryParams
    ); // 충돌 감지 함수 호출

    if (bResult) // 충돌 감지에 성공하면
    {
        ACharacter* PlayerCharacter = nullptr; // 첫 번째 감지된 플레이어 캐릭터 저장 변수

        for (auto const& OverlapResult : OverlapResults) // 충돌 감지된 액터들을 순회
        {
            PlayerCharacter = Cast<ACharacter>(OverlapResult.GetActor());
            if (IsValid(PlayerCharacter)) // 플레이어 캐릭터가 유효하면
            {
                // 첫 번째 플레이어만 공격 후 반복문 종료
                PlayerCharacter->TakeDamage(Strength, FDamageEvent(), GetController(), this);
             
                //UE_LOG(LogTemp, Warning, TEXT(" Player Health decreased to: %f"), Health);
                break;
            }
        }
    }

    DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Green, false, 5.f);
    //UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);
}

float ANXPlayerCharacter::GetHealth() const
{
    return Health;
}

void ANXPlayerCharacter::AddHealth(float Amount)
{
   
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
}

void ANXPlayerCharacter::UpdateHUD()
{
    if (!CharacterHUDWidget) return;

    // CharacterHUDWidget에서 'OverHeadHP'라는 이름을 가진 UTextBlock을 찾기
    if (UTextBlock* HPText = Cast<UTextBlock>(CharacterHUDWidget->GetWidgetFromName(TEXT("HPText"))))
    {
        // Health와 MaxHealth를 텍스트로 변환하여 설정
        HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
    }

    if (UTextBlock* TimeText = Cast<UTextBlock>(CharacterHUDWidget->GetWidgetFromName(TEXT("Time"))))
    {
        float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
        TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.0f"), RemainingTime)));
    }
}

void ANXPlayerCharacter::OnLevelTimeUp()
{
    EndLevel();
}

void ANXPlayerCharacter::EndLevel()
{
    GetWorldTimerManager().ClearTimer(LevelTimerHandle);
    // 다음 레벨 인덱스로
    CurrentLevelIndex++;

    // 모든 레벨을 다 돌았다면 게임 오버 처리
    if (CurrentLevelIndex >= MaxLevels)
    {
        OnGameOver();
        return;
    }

    else
    {
        // 맵 이름이 없으면 게임오버
        OnGameOver();
    }
}
void ANXPlayerCharacter::OnGameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
    IsDead();
   

}




void ANXPlayerCharacter::SetHUDWidget(UUserWidget* NewHUDWidget)
{
    CharacterHUDWidget = NewHUDWidget;
}



float ANXPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    float DamageCalculation(DamageAmount - Defense);
    if (DamageCalculation <= 0)
    {
        DamageCalculation = 0;
    }
    Health = FMath::Clamp(Health - DamageCalculation, 0.0f, MaxHealth);
    UpdateHUD();
    //UpdateOverheadHP();
    UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

    if (Health <= 0.0f)
    {
        IsDead();
    }

    return ActualDamage;
   
}

void ANXPlayerCharacter::IsDead()
{
   
    Destroy();
    UE_LOG(LogTemp, Warning, TEXT("IsDead")); 
}



void ANXPlayerCharacter::FireWeapon()
{
    if (CurrentWeapon)
    {
        
        CurrentWeapon->Fire();
    }
}

void ANXPlayerCharacter::ReloadWeapon()
{
    if (CurrentWeapon)
    {
       
        CurrentWeapon->Reload();
    }
}

void ANXPlayerCharacter::OnFirePressed()
{
    //FireWeapon();
  
    TArray<FOverlapResult> OverlapResults; // 충돌 감지 후 감지된 액터들을 담아 놓을 배열
    FCollisionQueryParams CollisionQueryParams(NAME_None, false, this); // 충돌 감지에 필요한 변수 선언
    bool bResult = GetWorld()->OverlapMultiByChannel(
        OverlapResults, GetActorLocation() + (GetActorForwardVector() * 100), FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(50.f), CollisionQueryParams
    ); // 충돌 감지 함수 호출

    if (bResult) // 충돌 감지에 성공하면
    {
        ACharacter* PlayerCharacter = nullptr; // 첫 번째 감지된 플레이어 캐릭터 저장 변수

        for (auto const& OverlapResult : OverlapResults) // 충돌 감지된 액터들을 순회
        {
            PlayerCharacter = Cast<ACharacter>(OverlapResult.GetActor());
            if (IsValid(PlayerCharacter)) // 플레이어 캐릭터가 유효하면
            {
                // 첫 번째 플레이어만 공격 후 반복문 종료
                PlayerCharacter->TakeDamage(Strength, FDamageEvent(), GetController(), this);

                //UE_LOG(LogTemp, Warning, TEXT("Player Health decreased to: %f"), Health);
                break;
            }
        }
    }

    DrawDebugSphere(GetWorld(), GetActorLocation() + (GetActorForwardVector()* 100), 50.f, 16, FColor::Green, false, 5.f);
    //UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);
}

void ANXPlayerCharacter::OnReloadPressed()
{
    ReloadWeapon();
}





void ANXPlayerCharacter::BeginCrouch(const FInputActionValue & value)
{
    ACharacter::Crouch();
}

void ANXPlayerCharacter::EndCrouch(const FInputActionValue & value)
{
    ACharacter::UnCrouch();
}

void ANXPlayerCharacter::Move(const FInputActionValue & value)
{
    if (!Controller) return;

    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void ANXPlayerCharacter::StartJump(const FInputActionValue & value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ANXPlayerCharacter::StopJump(const FInputActionValue & value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ANXPlayerCharacter::Look(const FInputActionValue & value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);

    AddControllerPitchInput(LookInput.Y);
}

void ANXPlayerCharacter::StartSprint(const FInputActionValue & value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ANXPlayerCharacter::StopSprint(const FInputActionValue & value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ANXPlayerCharacter::Interact(const FInputActionValue & value)
{

    if (value.Get<bool>())
    {

        if (!bHasKey)
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("You don't have a key!"));
            }
            return;
        }


        FVector Start = GetActorLocation();
        FVector End = Start + (GetActorForwardVector() * 200.f);

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);

        bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
        if (bHit)
        {
            ANXDoor* Door = Cast<ANXDoor>(Hit.GetActor());
            if (Door)
            {
                Door->OpenDoor();
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Opened the door!"));
                }
            }
            else
            {
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("No door found in front."));
                }
            }
        }
        else
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Nothing hit."));
            }
        }
    }
}
void ANXPlayerCharacter::InputAttack(const FInputActionValue & Invalue)
{
    //UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Attack()")));

    if (GetCharacterMovement()->IsFalling() == true)
    {

        return;
    }

    UNXCharacterAnimInstance* AnimInstance = Cast<UNXCharacterAnimInstance>(GetMesh()->GetAnimInstance());
    if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
    {
        AnimInstance->Montage_Play(AttackMontage);
    }
}

void ANXPlayerCharacter::InputFire(const FInputActionValue& Invalue)
{
    if (GetCharacterMovement()->IsFalling() == true)
    {
        
        return;
    }

    UNXCharacterAnimInstance* AnimInstance = Cast<UNXCharacterAnimInstance>(GetMesh()->GetAnimInstance());
    if (IsValid(AnimInstance) == true && IsValid(FireMontage) == true && AnimInstance->Montage_IsPlaying(FireMontage) == false)
    {
        OnFirePressed();
        AnimInstance->Montage_Play(FireMontage);
    }
}

void ANXPlayerCharacter::InputReload(const FInputActionValue& Invalue)
{
    if (GetCharacterMovement()->IsFalling() == true)
    {

        return;
    }

    UNXCharacterAnimInstance* AnimInstance = Cast<UNXCharacterAnimInstance>(GetMesh()->GetAnimInstance());
    if (IsValid(AnimInstance) == true && IsValid(ReloadMontage) == true && AnimInstance->Montage_IsPlaying(ReloadMontage) == false)
    {
        OnReloadPressed();
        AnimInstance->Montage_Play(ReloadMontage);
    }
}

///////////////////아이템과 연동//////////////////////


void ANXPlayerCharacter::IncreaseSpeed(float BoostAmount, float Duration)
{
 
    float OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;


    GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed + BoostAmount;

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, OriginalSpeed]()
        {
            GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
            UE_LOG(LogTemp, Warning, TEXT("Speed boost ended.."));
        },
        Duration,
        false
    );

    UE_LOG(LogTemp, Warning, TEXT("Speed increased by %f for %f seconds."), BoostAmount, Duration);
}



void ANXPlayerCharacter::IncreaseArmor(float AdditionalArmor, float Duration)
{

    float OriginalDefense = Defense;

    Defense += AdditionalArmor;

    UE_LOG(LogTemp, Warning, TEXT("Armor increased by %f. New defense: %f"), AdditionalArmor, Defense);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, OriginalDefense]()
        {
            Defense = OriginalDefense;
            UE_LOG(LogTemp, Warning, TEXT("Armor boost ended. Defense restored to %f"), OriginalDefense);
        },
        Duration,
        false
    );
}


void ANXPlayerCharacter::IncreaseAttack(float BoostAmount, float Duration)
{
  
    float OriginalStrength = Strength;


    Strength += BoostAmount;
    UE_LOG(LogTemp, Warning, TEXT("Attack increased by %f."), BoostAmount, Strength);

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle,
        [this, OriginalStrength]()
        {
            Strength = OriginalStrength;
            UE_LOG(LogTemp, Warning, TEXT("Attack boost ended."), OriginalStrength);
        },
        Duration,
        false
    );
}
