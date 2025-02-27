#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXArmorItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ANXPlayerCharacter;

UCLASS()
class NXPROJECT_API ANXArmorItem : public AActor
{
    GENERATED_BODY()

public:
    ANXArmorItem();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
    float ArmorValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
    float ArmorDuration;

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
