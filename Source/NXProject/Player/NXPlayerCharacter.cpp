#include "Player/NXPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "NXPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/NXDoor.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"

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
   
    //앉기
    bIsSitting = false;
    SitAnimMontage = nullptr;
    

    bHasKey = false;

    Tags.Add(FName("Player"));

}

void ANXPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    
}

void ANXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
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
            //앉기 
            if (PlayerController->SitAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SitAction,
                    ETriggerEvent::Completed,
                    this,
                    &ANXPlayerCharacter::StartSitting
                );

            }
            //일어나기
            if (PlayerController->StandAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->StandAction,
                    ETriggerEvent::Completed,
                    this,
                    &ANXPlayerCharacter::StopSitting
                );
            }
            

        }
    }
}



void ANXPlayerCharacter::StartSitting()
{
    if (bIsSitting)
    {
        return;
    }

    // 앉기 시작
    bIsSitting = true;

    // 애니메이션 몽타주 재생 (예: 앉기 애니메이션)
    if (SitAnimMontage && GetMesh())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(SitAnimMontage);
    }

    // 캐릭터의 높이를 낮추는 등 앉기 관련 설정
    GetCapsuleComponent()->SetCapsuleHalfHeight(44.0f); // 예시로 높이 조정
}

void ANXPlayerCharacter::StopSitting()
{
    if (!bIsSitting)
    {
        return;
    }

    // 앉기 상태 종료
    bIsSitting = false;

    // 애니메이션 복귀 (앉기 애니메이션이 끝나고 일어날 때)
    if (SitAnimMontage && GetMesh())
    {
        GetMesh()->GetAnimInstance()->Montage_Play(SitAnimMontage);
    }

    // 캐릭터 높이 원복
    GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
}



void ANXPlayerCharacter::Move(const FInputActionValue& value)
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

void ANXPlayerCharacter::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ANXPlayerCharacter::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ANXPlayerCharacter::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);

    AddControllerPitchInput(LookInput.Y);
}

void ANXPlayerCharacter::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ANXPlayerCharacter::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ANXPlayerCharacter::Interact(const FInputActionValue& value)
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