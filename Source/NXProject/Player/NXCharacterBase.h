#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NXCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class NXPROJECT_API ANXCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	
	ANXCharacterBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bHasKey;

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Ä«¸Þ¶ó
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	
    
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category= "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	
	
	bool bIsSitting;

	
	
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
