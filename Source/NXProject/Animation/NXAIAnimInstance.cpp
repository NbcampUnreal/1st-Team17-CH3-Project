// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NXAIAnimInstance.h"

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
