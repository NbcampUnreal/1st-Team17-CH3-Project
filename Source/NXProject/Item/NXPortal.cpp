#include "NXPortal.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ANXPortal::ANXPortal()
{
    PrimaryActorTick.bCanEverTick = false;

    PortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalTrigger"));
    RootComponent = PortalTrigger;
    PortalTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 150.0f));
    PortalTrigger->SetCollisionProfileName(TEXT("Trigger"));


    PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXPortal::OnOverlap);
}

void ANXPortal::BeginPlay()
{
    Super::BeginPlay();
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
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Portal Activated!"));
        }
        UE_LOG(LogTemp, Warning, TEXT("Moving to level: %s"), *NextLevelName.ToString());

        //다음 레벨로 넘어가기
        if (!NextLevelName.IsNone())
        {
            UGameplayStatics::OpenLevel(this, NextLevelName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NextLevelName is not set!"));
        }
    }
}

 









