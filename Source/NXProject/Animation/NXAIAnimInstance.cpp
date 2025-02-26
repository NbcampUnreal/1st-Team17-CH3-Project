// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NXAIAnimInstance.h"
#include "GameFramework/Character.h"
#include "MathUtil.h"
#include "GameFramework/CharacterMovementComponent.h"

void UNXAIAnimInstance::AnimNotify_CheckHit()
{
	if (OnCheckHit.IsBound() == true) // �ش� ��������Ʈ�� 1���� �Լ��� ���ε� �Ǿ� �ִٸ� true�� ��ȯ�ϴ� IsBound() �Լ� 
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
