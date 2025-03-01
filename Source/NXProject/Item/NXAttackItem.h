#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXAttackItem.generated.h"

UCLASS()
class NXPROJECT_API ANXAttackItem : public ANXBaseItem
{
    GENERATED_BODY()

public:
    ANXAttackItem();

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float AttackBoostAmount;

    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float BoostDuration;

   
    virtual void ActivateItem(AActor* Activator) override;
};
