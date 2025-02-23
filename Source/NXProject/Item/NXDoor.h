
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "NXDoor.generated.h"

class UStaticMeshComponent;
class UCurveFloat;

UCLASS()
class NXPROJECT_API ANXDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	ANXDoor();

UFUNCTION(BlueprintCallable)
void OpenDoor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleDoorMovement(float Value);

	bool bIsOpen;

	UPROPERTY(EditAnywhere,Category="Door")
	UStaticMeshComponent* DoorMesh;

	UPROPERTY()
	UTimelineComponent* OpenDoorTimeline;

	UPROPERTY(EditAnywhere, Category = "Door|Animation")
	UCurveFloat* OpenCurve;

	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Door|Animation")
	FVector TargetOffset;
};
