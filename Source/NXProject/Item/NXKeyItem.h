#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXKeyItem.generated.h"

UCLASS()
class NXPROJECT_API ANXKeyItem : public AActor
{
    GENERATED_BODY()

public:
    ANXKeyItem();

protected:
    virtual void BeginPlay() override;

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
