#include "Player/NXCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "NXPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ANXCharacterBase::ANXCharacterBase()
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

    bIsSitting = false;
    StandingHeight = 180.f;
    SittingHeight = 90.f;
}


void ANXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


void ANXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
                    &ANXCharacterBase::Move
                    );
                }
                if (PlayerController->JumpAction)
                {
                    
                        EnhancedInput->BindAction(
                        PlayerController->JumpAction,
                        ETriggerEvent::Triggered,
                        this,
                        &ANXCharacterBase::StartJump
                    );

                   
                    EnhancedInput->BindAction(
                        PlayerController->JumpAction,
                        ETriggerEvent::Completed,
                        this,
                        &ANXCharacterBase::StopJump
                    );
                }

                if (PlayerController->LookAction)
                {
                    
                    EnhancedInput->BindAction(
                        PlayerController->LookAction,
                        ETriggerEvent::Triggered,
                        this,
                        &ANXCharacterBase::Look
                    );
                }

                if (PlayerController->SprintAction)
                {

                    EnhancedInput->BindAction(
                        PlayerController->SprintAction,
                        ETriggerEvent::Triggered,
                        this,
                        &ANXCharacterBase::StartSprint
                    );

                    EnhancedInput->BindAction(
                        PlayerController->SprintAction,
                        ETriggerEvent::Completed,
                        this,
                        &ANXCharacterBase::StopSprint
                    );
                }
                if (PlayerController->SitAction)
                {

                    EnhancedInput->BindAction(
                        PlayerController->SprintAction,
                        ETriggerEvent::Triggered,
                        this,
                        &ANXCharacterBase::StartSit
                    );

                    EnhancedInput->BindAction(
                        PlayerController->SprintAction,
                        ETriggerEvent::Completed,
                        this,
                        &ANXCharacterBase::StopSit
                    );
                }
             
		}
	}

}



void ANXCharacterBase::Move(const FInputActionValue & value)
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

void ANXCharacterBase::StartJump(const FInputActionValue& value)
{
    if (value.Get<bool>())
    {
        Jump();
    }
}

void ANXCharacterBase::StopJump(const FInputActionValue& value)
{
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void ANXCharacterBase::Look(const FInputActionValue& value)
{
    FVector2D LookInput = value.Get<FVector2D>();

    AddControllerYawInput(LookInput.X);

    AddControllerPitchInput(LookInput.Y);
}

void ANXCharacterBase::StartSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ANXCharacterBase::StopSprint(const FInputActionValue& value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ANXCharacterBase::UpdateMovementSpeed()
{
    if (bIsSitting)
    {
        // 앉을 때 이동 속도 감소
        GetCharacterMovement()->MaxWalkSpeed = 300.f;
    }
    else
    {
        // 일어날 때 이동 속도 복구
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

void ANXCharacterBase::StartSit(const FInputActionValue& value)
{
    if (!bIsSitting)
    {
        bIsSitting = true;

        GetCapsuleComponent()->SetCapsuleHalfHeight(SittingHeight);

        UpdateMovementSpeed();

    }
}

void ANXCharacterBase::StopSit(const FInputActionValue& value)
{
    if (bIsSitting)
    {
        bIsSitting = false;

        GetCapsuleComponent()->SetCapsuleHalfHeight(StandingHeight);

        UpdateMovementSpeed();
    }
}

