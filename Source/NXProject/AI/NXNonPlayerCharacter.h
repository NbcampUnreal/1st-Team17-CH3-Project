#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NXNonPlayerCharacter.generated.h"


DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

class UAnimMontage;		// ���� Task ����

UCLASS()
class NXPROJECT_API ANXNonPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;		// ���� Task ����

public:
	ANXNonPlayerCharacter();		// ������

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCheckHit();

	virtual void IsDead() override;

	bool bIsNowAttacking;		// ���� Task ����B

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


protected:

	FTimerHandle AttackDamageTimerHandle;
	bool DeadEventDelayTime;


	virtual void BeginAttack();

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);		// ���� Task ����


//-------------------------------���� Task ����---------------------------------------//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;
	// �翬�� �̾߱�����, BP_NPC > Details > AttackMontage AM_TestAttack ���� �ʼ�.

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;
	// �ִϸ��̼� ��Ÿ�� ����� ������ �� ȣ���� ��������Ʈ.

//-------------------------------------------------------------------------------------//
};

