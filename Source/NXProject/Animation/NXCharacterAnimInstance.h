#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NXCharacterAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheck); // ��������Ʈ �ڷ��� ����


UCLASS()
class NXPROJECT_API UNXCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void AnimNotify_CheckHit();

public:
	FOnCheck OnCheckHit; // ������ ������ ��������Ʈ �ڷ����� ���� ��������Ʈ ��ü ����.
	
};
