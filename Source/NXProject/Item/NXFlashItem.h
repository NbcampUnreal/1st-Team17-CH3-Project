
#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXFlashItem.generated.h"

class USpotLightComponent;
class USphereComponent;
class ANXPlayerCharacter;
class ANXNonPlayerCharacter;

UCLASS()
class NXPROJECT_API ANXFlashItem : public ANXBaseItem
{
    GENERATED_BODY()

public:
    ANXFlashItem();

    virtual void BeginPlay() override;
    virtual void ActivateItem(AActor* Activator) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USpotLightComponent* Flashlight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    float LightDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    float NonPlayerCharacterFreezeDuration;

    bool bIsActive; 

    void DeactivateFlashlight();
    void UnfreezeNonPlayerCharacter();

    TArray<ANXNonPlayerCharacter*> FrozenNonPlayerCharacter;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* PickupSound;

private:
    FTimerHandle FlashlightTimerHandle;
    FTimerHandle NPCFreezeTimerHandle;


};
