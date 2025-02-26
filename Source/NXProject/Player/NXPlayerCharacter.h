#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "NXPlayerCharacter.generated.h"

class AWeaponBase;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;

UCLASS()
class NXPROJECT_API ANXPlayerCharacter : public ANXCharacterBase
{
	GENERATED_BODY()

public:
	ANXPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bHasKey;

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//************공격************

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UFUNCTION()
	void OnCheckHit();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	bool bIsDead;

	void IsDead();
	
	//************카메라************

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	//************캐릭터무브먼트************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;


	//************무기************

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponBase* EquippedWeapon; // 소지한 무기

	// 발사, 재장전 함수
	void FireWeapon();
	void ReloadWeapon();

	// 입력 처리 함수
	void OnFirePressed();
	void OnReloadPressed();
	// 무기 소지 여부
	bool bIsReloading;

	
	

	//************앉기************

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsSitting;
	// 앉기 액션을 토글하는 함수
	void BeginCrouch(const FInputActionValue& value);
	void EndCrouch(const FInputActionValue& value);
	

	

	

	//************캐릭터 실제 구현 함수************
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




	
};
