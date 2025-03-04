#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "NXPlayerCharacter.generated.h"


class AWeaponBase;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UWidgetComponent;


UCLASS()
class NXPROJECT_API ANXPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

public:
	ANXPlayerCharacter();

	///////////������/////////////
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bHasKey;
	UFUNCTION(BlueprintCallable, Category = "Key")
	void PickupKey();

	UFUNCTION(BlueprintCallable, Category = "Speed")
	void IncreaseSpeed(float BoostAmount, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Armor")
	void IncreaseArmor(float AdditionalArmor, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void IncreaseAttack(float AdditionalArmor, float Duration);

	void SetHUDWidget(UUserWidget* NewHUDWidget);

	virtual float GetHealth() const override;

	virtual void AddHealth(float Amount) override;

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//************����************

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UFUNCTION()
	void OnCheckHit();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bIsDead;

	void IsDead();
	
	//************ī�޶�,����************

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;


	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "UI")
	UUserWidget* CharacterHUDWidget;

	void UpdateHUD();

	FTimerHandle HUDUpdateTimerHandle;
	FTimerHandle LevelTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	void OnLevelTimeUp();
	void EndLevel();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UFUNCTION(BlueprintCallable, Category = "Game")
	void OnGameOver();


	//************ĳ���͹����Ʈ************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;


	//************����************


	// �߻�, ������ �Լ�
	void FireWeapon();
	void ReloadWeapon();

	// �Է� ó�� �Լ�
	void OnFirePressed();
	void OnReloadPressed();
	

	// �ѱ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponBase* CurrentWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category= "Weapon")
	TSubclassOf<AWeaponBase> Weaponclass;

	//************�ɱ�************

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsSitting;
	// �ɱ� �׼��� ����ϴ� �Լ�
	void BeginCrouch(const FInputActionValue& value);
	void EndCrouch(const FInputActionValue& value);

	

	

	

	//************ĳ���� ���� ���� �Լ�************
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void Interact(const FInputActionValue& value);
	UFUNCTION()
	void InputAttack(const FInputActionValue& Invalue);
	UFUNCTION()
	void InputFire(const FInputActionValue& Invalue);
	UFUNCTION()
	void InputReload(const FInputActionValue& Invalue);

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintOnDeath();

	
};
