#include "NXDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ANXDoor::ANXDoor()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsOpen = false;
    TargetOffset = FVector(0.f, 2000.f, 0.f); 

    DoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTrigger"));
    RootComponent = DoorTrigger;
    DoorTrigger->SetBoxExtent(FVector(100.f, 100.f, 150.f));
    DoorTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DoorTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    DoorTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    DoorTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    DoorTrigger->SetGenerateOverlapEvents(true);
    DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXDoor::OnOverlap);


    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);

    OpenDoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenDoorTimeline"));
}

void ANXDoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedLocation = GetActorLocation();

    if (OpenCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("HandleDoorMovement"));
        OpenDoorTimeline->AddInterpFloat(OpenCurve, TimelineCallback);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OpenCurve is not set in %s"), *GetName());
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

void ANXDoor::HandleDoorMovement(float Value)
{

    FVector NewLocation = FMath::Lerp(ClosedLocation, ClosedLocation + TargetOffset, Value);
    SetActorLocation(NewLocation);
}


void ANXDoor::OpenDoor()
{
    if (!bIsOpen && OpenCurve)
    {
        bIsOpen = true;
        OpenDoorTimeline->PlayFromStart();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Door Opened!"));
        }
        UE_LOG(LogTemp, Warning, TEXT("Door Opened! Moving from %s to %s"), *ClosedLocation.ToString(), *(ClosedLocation + TargetOffset).ToString());
    }
    else if (bIsOpen)
    {
        UE_LOG(LogTemp, Warning, TEXT("Door is already open."));
    }
}

void ANXDoor::OnOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!OtherActor)
        return;

    UE_LOG(LogTemp, Warning, TEXT("Door Trigger Overlap with: %s"), *OtherActor->GetName());

    
    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player detected in door trigger. Opening door in 1 seconds."));

        GetWorld()->GetTimerManager().SetTimer(DoorOpenDelayTimerHandle, this, &ANXDoor::OpenDoor, 1.0f, false);
    }
}
