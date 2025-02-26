#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NXCharacterAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheck); // 델리게이트 자료형 정의


UCLASS()
class NXPROJECT_API UNXCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void AnimNotify_CheckHit();

public:
	FOnCheck OnCheckHit; // 위에서 정의한 델리게이트 자료형을 통해 델리게이트 개체 선언.
	
};
