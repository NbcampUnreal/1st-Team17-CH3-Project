#include "NXPortal.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

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
    if (!OtherActor) return;

    // 플레이어만 감지 (ACharacter 기반)
    if (OtherActor->IsA(ACharacter::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player entered the portal: %s"), *GetNameSafe(OtherActor));

        if (!NextLevelName.IsNone())
        {
            UE_LOG(LogTemp, Warning, TEXT("Moving to level: %s"), *NextLevelName.ToString());
            UGameplayStatics::OpenLevel(this, NextLevelName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NextLevelName is not set!"));
        }
    }
}








