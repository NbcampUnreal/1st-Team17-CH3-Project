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

	void ApplyAttackDamage();		// 데미지 적용 함수

	bool bIsNowAttacking;		// 공격 Task 구현

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Strength;		// 공격력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float MaxHP;	// 최대 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float CurrentHP;	// 현재 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Defense;		// 방어력

	virtual float TakeDamage
	(
		float DamageAmount,					// 받은 데미지 양
		FDamageEvent const& DamageEvent,	// 데미지 이벤트 정보
		AController* EventInstigator,		// 데미지를 가한 컨트롤러
		AActor* DamageCauser				// 데미지를 가한 액터
	) override;

	bool IsDead() const;	// 체력이 0 이하인지 확인

protected:

	FTimerHandle AttackDamageTimerHandle;

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

