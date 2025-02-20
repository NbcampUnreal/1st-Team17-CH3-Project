#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXMineItem.generated.h"

UCLASS()
class NXPROJECT_API ANXMineItem : public ANXBaseItem
{
    GENERATED_BODY()

public:
    ANXMineItem();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDamage;

    virtual void ActivateItem(AActor* Activator) override;
};
