#include "NXRooftopDoor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ANXRooftopDoor::ANXRooftopDoor()
{
    PrimaryActorTick.bCanEverTick = true;
    bIsOpen = false;


    TargetOffset = FVector(700.f, 0.f, 0.f);


    DoorTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorTrigger"));
    RootComponent = DoorTrigger;
    DoorTrigger->SetBoxExtent(FVector(100.f, 100.f, 150.f));
    DoorTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DoorTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    DoorTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    DoorTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    DoorTrigger->SetGenerateOverlapEvents(true);
    DoorTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXRooftopDoor::OnOverlap);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);


    OpenDoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenDoorTimeline"));
}

void ANXRooftopDoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedLocation = GetActorLocation();

    if (OpenCurve)
    {
        FOnTimelineFloat TimelineCallback;
        TimelineCallback.BindUFunction(this, FName("HandleDoorMovement"));
        OpenDoorTimeline->AddInterpFloat(OpenCurve, TimelineCallback);

      
        OpenDoorTimeline->SetLooping(false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OpenCurve is not set: %s"), *GetName());
    }
}

void ANXRooftopDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OpenDoorTimeline)
    {
        OpenDoorTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
    }
}

void ANXRooftopDoor::HandleDoorMovement(float Value)
{
   
    FVector NewLocation = FMath::Lerp(ClosedLocation, ClosedLocation + TargetOffset, Value);
    SetActorLocation(NewLocation);
}

void ANXRooftopDoor::OpenDoor()
{
    if (!bIsOpen && OpenCurve)
    {
        bIsOpen = true;
        OpenDoorTimeline->PlayFromStart();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Rooftop door opened!"));
        }
        UE_LOG(LogTemp, Warning, TEXT("Rooftop door opened: Closed location %s -> Open location %s"),
            *ClosedLocation.ToString(), *(ClosedLocation + TargetOffset).ToString());
    }
    else if (bIsOpen)
    {
        UE_LOG(LogTemp, Warning, TEXT("Rooftop door is already open."));
    }
}

void ANXRooftopDoor::OnOverlap(
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

    UE_LOG(LogTemp, Warning, TEXT("Rooftop door trigger overlap: %s"), *OtherActor->GetName());


    if (OtherActor->ActorHasTag("Player"))
    {
      
        if (OtherActor->ActorHasTag("HasKey"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Player has the key item. Opening rooftop door."));

            if (PickupSound)
            {
                UGameplayStatics::PlaySoundAtLocation(
                    GetWorld(),
                    PickupSound,
                    GetActorLocation()
                );
            }
            GetWorld()->GetTimerManager().SetTimer(DoorOpenDelayTimerHandle, this, &ANXRooftopDoor::OpenDoor, 1.0f, false);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Player does not have the key item. Cannot open rooftop door."));
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Key item is required!"));
            }
        }
    }
}
///
