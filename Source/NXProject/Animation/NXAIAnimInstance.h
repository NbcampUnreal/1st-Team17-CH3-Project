// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NXAIAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHit); // 델리게이트 자료형 정의
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
	FOnCheckHit OnCheckHit; // 위에서 정의한 델리게이트 자료형을 통해 델리게이트 개체 선언.

};
