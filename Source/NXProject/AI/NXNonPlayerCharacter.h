#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NXNonPlayerCharacter.generated.h"

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool /*bInterrupted*/)
DECLARE_DELEGATE_TwoParams(FOnDeadMontageEnded, UAnimMontage*, bool /*bInterrupted*/)

class UAnimMontage;		// ���� Task ����
class UWidgetComponent;		// �̸�ǥ

UCLASS()
class NXPROJECT_API ANXNonPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;		// ���� Task ����

public:
	ANXNonPlayerCharacter();		// ������
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* AINameplateWidget;		//�̸�ǥ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* AIHealtBar;		//ü�¹�

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCheckHit();

	UFUNCTION()
	void IsDead();


	bool bIsNowAttacking;		// ���� Task ����B

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void UpdateAINameplate();
	void UpdateAIHealtBar();

protected:

	FTimerHandle AttackDamageTimerHandle;
	FTimerHandle DeadTimerHandle;

	virtual void BeginDead();

	virtual void EndDead(UAnimMontage* InMontage, bool bInterruped);


	virtual void BeginAttack();

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);		// ���� Task ����

//-------------------------------���� Task ����---------------------------------------//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> AttackMontage;
	// �翬�� �̾߱�����, BP_NPC > Details > AttackMontage AM_TestAttack ���� �ʼ�.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UAnimMontage> DeadMontage;

	FOnAttackMontageEnded OnAttackMontageEndedDelegate;
	FOnDeadMontageEnded OnDeadMontageEndedDelegate;
	// �ִϸ��̼� ��Ÿ�� ����� ������ �� ȣ���� ��������Ʈ.

//-------------------------------------------------------------------------------------//
};

