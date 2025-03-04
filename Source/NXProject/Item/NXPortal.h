#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NXPortal.generated.h"

class UBoxComponent;

UCLASS()
class NXPROJECT_API ANXPortal : public AActor
{
    GENERATED_BODY()

public:
    ANXPortal();

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* PickupSound;

    UFUNCTION()
    void MoveToNextLevel();

    bool bActivated;


public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    UBoxComponent* PortalTrigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
    FName NextLevelName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    UNiagaraSystem* PickupNiagara;
};
