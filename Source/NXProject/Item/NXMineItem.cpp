#include "NXMineItem.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

ANXMineItem::ANXMineItem()
{
    ExplosionDelay = 2.0f;
    ExplosionRadius = 300.0f;
    ExplosionDamage = 30.0f;
    ItemType = "Mine";

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(RootComponent);
}

void ANXMineItem::ActivateItem(AActor* Activator)
{
   
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ANXMineItem::Explode, ExplosionDelay, false);
}

void ANXMineItem::Explode()
{
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %f by MineItem"), ExplosionDamage));
            }
        }
    }

    Destroy();
}
