#include "NXMineItem.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Player/NXPlayerCharacter.h" 
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"

ANXMineItem::ANXMineItem()
{
    ExplosionDelay = 0.5f;
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
            if (PickupSound)
            {
                UGameplayStatics::PlaySoundAtLocation(
                    GetWorld(),
                    PickupSound,
                    GetActorLocation()
                );
            }
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player damaged %f by MineItem"), ExplosionDamage));
            }
            ACharacter* PlayerCharacter = Cast<ACharacter>(Actor);
            if (PlayerCharacter)
            {
                PlayerCharacter->TakeDamage(ExplosionDamage, FDamageEvent(), nullptr, this);
            }
        }
    }
    UNiagaraComponent* NiagaraComponent = nullptr;
    if (PickupNiagara)
    {
        NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            PickupNiagara,
            GetActorLocation(),
            GetActorRotation(),
            FVector(1.0f),
            false,
            true,
            ENCPoolMethod::None
        );
    }


    if (NiagaraComponent)
    {
        FTimerHandle DestroyNiagaraTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DestroyNiagaraTimerHandle,
            [NiagaraComponent]()
            {
                NiagaraComponent->DestroyComponent();
            },
            2.0f,
            false
        );
    }

    Destroy();
}
