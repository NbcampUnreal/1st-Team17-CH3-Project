
#pragma once
#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXMineItem.generated.h"

class USphereComponent;

UCLASS()
class NXPROJECT_API ANXMineItem : public ANXBaseItem
{
    GENERATED_BODY()

public:
    ANXMineItem();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* ExplosionCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDamage;

    FTimerHandle ExplosionTimerHandle;

    virtual void ActivateItem(AActor* Activator) override;

    UFUNCTION()
    void Explode();
};
