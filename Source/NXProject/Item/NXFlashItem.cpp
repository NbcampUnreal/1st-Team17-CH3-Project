#include "NXFlashItem.h"
#include "Player/NXPlayerCharacter.h"
#include "AI/NXNonPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ANXFlashItem::ANXFlashItem()
{
    LightDuration = 5.0f;
    NonPlayerCharacterFreezeDuration = 3.0f;
    bIsActive = false; 

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(500.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
    Flashlight->SetupAttachment(RootComponent);
    Flashlight->SetVisibility(false);
    Flashlight->Intensity = 10000.0f;
    Flashlight->SetLightColor(FLinearColor(1.0f, 1.0f, 0.8f));
    Flashlight->AttenuationRadius = 800.0f;
    Flashlight->OuterConeAngle = 40.0f;
}

void ANXFlashItem::BeginPlay()
{
    Super::BeginPlay();
}

void ANXFlashItem::ActivateItem(AActor* Activator)
{
    ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(Activator);


    bIsActive = true;  
    Flashlight->SetVisibility(true);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Flashlight activated!"));
    }

    GetWorld()->GetTimerManager().SetTimer(
        FlashlightTimerHandle, this, &ANXFlashItem::DeactivateFlashlight, LightDuration, false);

    TArray<AActor*> OverlappingActors;
    CollisionComponent->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        ANXNonPlayerCharacter* NPC = Cast<ANXNonPlayerCharacter>(Actor);
        if (NPC)
        {
            NPC->FreezeMovement();
            FrozenNonPlayerCharacter.Add(NPC);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        NPCFreezeTimerHandle, this, &ANXFlashItem::UnfreezeNonPlayerCharacter, NonPlayerCharacterFreezeDuration, false);
}

void ANXFlashItem::DeactivateFlashlight()
{
    Flashlight->SetVisibility(false);
    bIsActive = false; 
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Flashlight deactivated!"));
    }
}
void ANXFlashItem::UnfreezeNonPlayerCharacter()
{
    for (ANXNonPlayerCharacter* NPC : FrozenNonPlayerCharacter)
    {
        if (NPC)
        {
            NPC->UnfreezeMovement();
        }
    }
    FrozenNonPlayerCharacter.Empty();
}
