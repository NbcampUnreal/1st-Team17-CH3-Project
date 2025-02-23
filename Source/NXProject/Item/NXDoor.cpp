
#include "Item/NXDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"


ANXDoor::ANXDoor()
{
 
	PrimaryActorTick.bCanEverTick = true;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;

	OpenDoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenDoorTimeline"));
	OpenDoorTimeline->SetLooping(false);

	bIsOpen = false;

	//문이 열리면 위로 200 이동
	TargetOffset = FVector(0.f, 0.f, 200.f);


}

void ANXDoor::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = DoorMesh->GetRelativeLocation();

	if (OpenCurve)
	{
		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName("HandleDoorMovement"));
		OpenDoorTimeline->AddInterpFloat(OpenCurve, TimelineCallback);
	}
}

void ANXDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OpenDoorTimeline)
	{
		OpenDoorTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	}
}

void ANXDoor::OpenDoor()
{
	if (!bIsOpen)
	{
		bIsOpen = true;

		if (OpenDoorTimeline && OpenCurve)
		{
			OpenDoorTimeline->PlayFromStart();
		}
		else
		{
			DoorMesh->SetRelativeLocation(StartLocation + TargetOffset);
		}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Congratulations! The door has opened!"));
		}
	}
}

void ANXDoor::HandleDoorMovement(float Value)
{
	FVector NewLocation = FMath::Lerp(StartLocation, StartLocation + TargetOffset, Value);
	DoorMesh->SetRelativeLocation(NewLocation);
}

