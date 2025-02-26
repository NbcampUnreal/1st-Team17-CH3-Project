// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NXAIAnimInstance.h"

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
