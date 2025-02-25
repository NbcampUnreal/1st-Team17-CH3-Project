#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WJH_Character.generated.h"

UCLASS()
class NXPROJECT_API AWJH_Character : public ACharacter
{
    GENERATED_BODY()

public:
    AWJH_Character();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void Interact(const FInputActionValue& Value);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bHasKey;

    UFUNCTION(BlueprintCallable)
    void SetHasKey(bool HasKey) { bHasKey = HasKey; }

    UFUNCTION(BlueprintCallable)
    bool GetHasKey() const { return bHasKey; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    class USpringArmComponent* SpringArmComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    class UCameraComponent* CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float NormalSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeedMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed;

    UFUNCTION()
    void StartSprint(const FInputActionValue& Value);

    UFUNCTION()
    void StopSprint(const FInputActionValue& Value);

    UFUNCTION()
    void Move(const FInputActionValue& Value);

    UFUNCTION()
    void StartJump(const FInputActionValue& Value);

    UFUNCTION()
    void StopJump(const FInputActionValue& Value);

    UFUNCTION()
    void Look(const FInputActionValue& Value);
};
