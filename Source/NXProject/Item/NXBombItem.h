
#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXBombItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class NXPROJECT_API ANXBombItem : public ANXBaseItem
{
    GENERATED_BODY()

public:
    
    ANXBombItem();

    UFUNCTION(BlueprintCallable, Category = "Item")
    void ThrowItem(const FVector& LaunchVelocity);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine|Component")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mine|Component")
    USphereComponent* ExplosionCollision;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDamage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    UProjectileMovementComponent* ProjectileMovement;

    FTimerHandle ExplosionTimerHandle;

    virtual void ActivateItem(AActor* Activator) override;

    void Explode();
};
