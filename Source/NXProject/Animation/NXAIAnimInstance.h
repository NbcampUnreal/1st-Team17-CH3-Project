// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NXAIAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckHit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIsDead);// ��������Ʈ �ڷ��� ����
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

	UFUNCTION()
	void AnimNotify_IsDead();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ACharacter> Character;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	FOnCheckHit OnCheckHit; // ������ ������ ��������Ʈ �ڷ����� ���� ��������Ʈ ��ü ����.
	FIsDead IsDead;
};
