// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NXAIAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHit); // ��������Ʈ �ڷ��� ����
/**
 * 
 */
UCLASS()
class NXPROJECT_API UNXAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UFUNCTION()
	void AnimNotify_CheckHit();

public:
	FOnCheckHit OnCheckHit; // ������ ������ ��������Ʈ �ڷ����� ���� ��������Ʈ ��ü ����.

};
