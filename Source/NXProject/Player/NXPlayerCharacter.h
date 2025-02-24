#pragma once

#include "CoreMinimal.h"
#include "Player/NXCharacterBase.h"
#include "NXPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;


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

	//ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	//�ɱⱸ��
	 // �ɱ� ������ ó���ϴ� �Լ�
	void StartSitting();
	void StopSitting();

	// �ɱ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsSitting;

	// �ִϸ��̼� ��Ÿ�� (�ɱ� �ִϸ��̼�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* SitAnimMontage;

	// �Է¿� ������ �Լ�
	void OnCrouchPressed();
	
	

	
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





	
};
