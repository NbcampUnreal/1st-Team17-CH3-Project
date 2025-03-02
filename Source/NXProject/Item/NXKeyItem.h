#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NXKeyItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class NXPROJECT_API ANXKeyItem : public AActor
{
    GENERATED_BODY()

public:
    ANXKeyItem();


    virtual void ActivateItem(AActor* Activator);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
    UStaticMeshComponent* KeyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Key Item")
    USphereComponent* CollisionComponent;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    UNiagaraSystem* PickupNiagara;

    UFUNCTION()
    void OnOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );
};
