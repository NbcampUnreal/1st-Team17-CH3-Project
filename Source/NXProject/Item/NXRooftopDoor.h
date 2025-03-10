#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXRooftopDoor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTimelineComponent;
class UCurveFloat;

UCLASS()
class NXPROJECT_API ANXRooftopDoor : public AActor
{
    GENERATED_BODY()

public:

    ANXRooftopDoor();

protected:
  
    virtual void BeginPlay() override;

  
    virtual void Tick(float DeltaTime) override;


    UFUNCTION()
    void HandleDoorMovement(float Value);


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
