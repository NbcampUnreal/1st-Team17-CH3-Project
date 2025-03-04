#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXDoor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class NXPROJECT_API ANXDoor : public AActor
{
    GENERATED_BODY()

public:
    ANXDoor();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void HandleDoorMovement(float Value);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* PickupSound;

    UFUNCTION()
    void OnOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );



public:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    UBoxComponent* DoorTrigger;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    UStaticMeshComponent* DoorMesh;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    UTimelineComponent* OpenDoorTimeline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Animation")
    UCurveFloat* OpenCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Animation")
    FVector TargetOffset;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    bool bIsOpen;


    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();



    FTimerHandle DoorOpenDelayTimerHandle;

private:

    FVector ClosedLocation;


};
