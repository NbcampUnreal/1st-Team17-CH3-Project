#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXDoor.generated.h"

UCLASS()
class NXPROJECT_API ANXDoor : public AActor
{
    GENERATED_BODY()

public:
    ANXDoor();
    void OpenDoor();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
    UStaticMeshComponent* DoorMesh;
};
