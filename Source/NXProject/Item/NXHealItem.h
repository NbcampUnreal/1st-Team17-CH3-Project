#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXHealItem.generated.h"

UCLASS()
class NXPROJECT_API ANXHealItem : public ANXBaseItem
{
    GENERATED_BODY()

public:
    ANXHealItem();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float HealAmount;

    virtual void ActivateItem(AActor* Activator) override;
};
