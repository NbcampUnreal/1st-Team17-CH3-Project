#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXItemInterface.h"  
#include "NiagaraSystem.h"         
#include "NiagaraFunctionLibrary.h"
#include "NXBaseItem.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class NXPROJECT_API ANXBaseItem : public AActor, public INXItemInterface
{
    GENERATED_BODY()

public:
    ANXBaseItem();

protected:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USceneComponent* Scene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* Collision;

 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UNiagaraSystem* PickupNiagara; //¿Ã∆Â∆Æ

   
    UFUNCTION()
    virtual void OnItemOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    ) override;

    UFUNCTION()
    virtual void OnItemEndOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    ) override;

    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual void ActivateItem(AActor* Activator) override;

    virtual FName GetItemType() const override;

    UFUNCTION()
    void DestroyItem();
};
