#include "WJH_Character.h"
#include "EnhancedInputComponent.h"
#include "Player/NXPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Item/NXDoor.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

AWJH_Character::AWJH_Character()
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

    bHasKey = false;
}

void AWJH_Character::BeginPlay()
{
    Super::BeginPlay();
}

void AWJH_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
                    &AWJH_Character::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AWJH_Character::StartJump
                );

                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AWJH_Character::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AWJH_Character::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AWJH_Character::StartSprint
                );

                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &AWJH_Character::StopSprint
                );
            }

            if (PlayerController->InteractAction)
            {
                EnhancedInput->BindAction(
                    PlayerController->InteractAction,
                    ETriggerEvent::Completed,
                    this,
                    &AWJH_Character::Interact
                );
            }
        }
    }
}

void AWJH_Character::Move(const FInputActionValue& Value)
{
    if (!Controller) return;

    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AWJH_Character::StartJump(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        Jump();
    }
}

void AWJH_Character::StopJump(const FInputActionValue& Value)
{
    if (!Value.Get<bool>())
    {
        StopJumping();
    }
}

void AWJH_Character::Look(const FInputActionValue& Value)
{
    FVector2D LookInput = Value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);
    AddControllerPitchInput(LookInput.Y);
}

void AWJH_Character::StartSprint(const FInputActionValue& Value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AWJH_Character::StopSprint(const FInputActionValue& Value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void AWJH_Character::Interact(const FInputActionValue& Value)
{
    if (!bHasKey)
    {
        UE_LOG(LogTemp, Warning, TEXT("키 없음!"));
        return;
    }

    FVector Start = GetActorLocation();
    FVector End = Start + (GetActorForwardVector() * 200.f);

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 2.0f);

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
    if (bHit)
    {
        ANXDoor* Door = Cast<ANXDoor>(Hit.GetActor());
        if (Door)
        {
            Door->OpenDoor();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("감지된 오브젝트가 문이 아님: %s"), *Hit.GetActor()->GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("아무것도 감지되지 않음."));
    }
}
