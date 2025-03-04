
#pragma once

#include "CoreMinimal.h"
#include "Item/NXHealItem.h"
#include "NXBigHealItem.generated.h"

UCLASS()
class NXPROJECT_API ANXBigHealItem : public ANXHealItem
{
    GENERATED_BODY()

public:
    ANXBigHealItem();

    virtual void ActivateItem(AActor* Activator) override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* PickupSound;
};
