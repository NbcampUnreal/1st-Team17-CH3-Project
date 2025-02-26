#include "Player/NXPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "NXPlayerController.h"
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



ANXPlayerCharacter::ANXPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

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
    
    MaxHealth = 1000;
    Strength = 15;
    
    Defense = 1;
    bIsSitting = false;
    bIsDead = false;
    bIsReloading = false;

    

    bHasKey = false;

    Tags.Add(FName("Player"));

}

void ANXPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

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

float ANXPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    float DamageCalculation(DamageAmount - Defense);
    if (DamageCalculation <= 0)
    {
        DamageCalculation = 0;
    }
    Health = FMath::Clamp(Health - DamageCalculation, 0.0f, MaxHealth);
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
    if (EquippedWeapon && !bIsReloading)
    {
        EquippedWeapon->Fire();
    }
}

void ANXPlayerCharacter::ReloadWeapon()
{
    if (EquippedWeapon && !bIsReloading)
    {
        bIsReloading = true;
        EquippedWeapon->Reload();
    }
}

void ANXPlayerCharacter::OnFirePressed()
{
    FireWeapon();
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






































































































































































































































































































































































































































































































































































































































































































































