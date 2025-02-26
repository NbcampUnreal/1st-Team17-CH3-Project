#include "NXPortal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ANXPortal::ANXPortal()
{
    PrimaryActorTick.bCanEverTick = false;


    PortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalTrigger"));
    RootComponent = PortalTrigger;
    PortalTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 150.0f));

    PortalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PortalTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    PortalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    PortalTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PortalTrigger->SetGenerateOverlapEvents(true);

  
    PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXPortal::OnOverlap);
}

void ANXPortal::BeginPlay()
{
    Super::BeginPlay();

 
    UE_LOG(LogTemp, Warning, TEXT("NXPortal Initialized. Next Level: %s"), *NextLevelName.ToString());
}

void ANXPortal::OnOverlap(
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

    UE_LOG(LogTemp, Warning, TEXT("NXPortal: Overlap detected with actor: %s"), *OtherActor->GetName());

    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Warning, TEXT("NXPortal: Player detected. Moving to level: %s"), *NextLevelName.ToString());
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NXPortal Activated!"));
        }
        if (!NextLevelName.IsNone())
        {
            UGameplayStatics::OpenLevel(this, NextLevelName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NXPortal Error: NextLevelName is not set!"));
        }
    }
}
