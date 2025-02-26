#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NXNonPlayerCharacter.generated.h"


DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

class UAnimMontage;		// 공격 Task 구현

UCLASS()
class NXPROJECT_API ANXNonPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;		// 공격 Task 구현

public:
	ANXNonPlayerCharacter();		// 생성자

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCheckHit();

	virtual void IsDead() override;

	bool bIsNowAttacking;		// 공격 Task 구현B

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:

	FTimerHandle AttackDamageTimerHandle;
	bool DeadEventDelayTime;


	virtual void BeginAttack();

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);		// 공격 Task 구현


//-------------------------------공격 Task 구현---------------------------------------//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;
	// 당연한 이야기지만, BP_NPC > Details > AttackMontage AM_TestAttack 지정 필수.

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;
	// 애니메이션 몽타주 재생이 끝났을 때 호출할 델리게이트.

//-------------------------------------------------------------------------------------//
};

