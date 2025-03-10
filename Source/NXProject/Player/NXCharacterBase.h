#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NXCharacterBase.generated.h"

class UAnimMontage;

UCLASS()
class NXPROJECT_API ANXCharacterBase : public ACharacter
{
	GENERATED_BODY()



public:
	ANXCharacterBase();
	
	UFUNCTION(BlueprintPure, Category = "Health")
	virtual float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void AddHealth(float Amount);



protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Strength;		// 공격력UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	float Defense;		// 방어력


};
