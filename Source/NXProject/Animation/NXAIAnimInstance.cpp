// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NXAIAnimInstance.h"
#include "GameFramework/Character.h"
#include "MathUtil.h"
#include "GameFramework/CharacterMovementComponent.h"

void UNXAIAnimInstance::AnimNotify_CheckHit()
{
	if (OnCheckHit.IsBound() == true) // 해당 델리게이트에 1개의 함수라도 바인드 되어 있다면 true를 반환하는 IsBound() 함수 
	{
		OnCheckHit.Broadcast();
	}
}

void UNXAIAnimInstance::AnimNotify_IsDead()
{
	if (IsDead.IsBound() == true)
	{
		IsDead.Broadcast();
	}
}

void UNXAIAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (IsValid(Character))
	{
		MovementComponent = Character->GetCharacterMovement();
	}
}

void UNXAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(MovementComponent))
	{
		Speed = MovementComponent->Velocity.Size();
	}
}
