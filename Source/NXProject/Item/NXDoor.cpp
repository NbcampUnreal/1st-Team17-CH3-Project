#include "NXDoor.h"
#include "Components/StaticMeshComponent.h"

ANXDoor::ANXDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    RootComponent = DoorMesh;
}

void ANXDoor::BeginPlay()
{
    Super::BeginPlay();
}

void ANXDoor::OpenDoor()
{
    FVector NewLocation = GetActorLocation();
    NewLocation.Z += 200.0f;
    SetActorLocation(NewLocation);
}
