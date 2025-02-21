
#include "Item/NXMineItem.h"
#include "Components/SphereComponent.h"

ANXMineItem::ANXMineItem()
{
    ExplosionDelay = 0.5f;
    ExplosionRadius = 250;
    ExplosionDamage = 30.0f;
    ItemType = "Mine";

    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);
}

void ANXMineItem::ActivateItem(AActor* Activator)
{
    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ANXMineItem::Explode, ExplosionDelay);
}
void ANXMineItem::Explode()
{
    TArray<AActor*>OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("Player"))
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ZOMBIE damage %d by MineItem"), ExplosionDamage));

        }
    }

    DestroyItem();
}
