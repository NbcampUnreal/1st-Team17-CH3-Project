#pragma once

#include "CoreMinimal.h"
#include "Item/NXHealItem.h"
#include "NXSmallHealItem.generated.h"

UCLASS()
class NXPROJECT_API ANXSmallHealItem : public ANXHealItem
{
    GENERATED_BODY()

public:
    ANXSmallHealItem();

    virtual void ActivateItem(AActor* Activator) override;
};
