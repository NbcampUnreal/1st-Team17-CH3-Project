
#include "Item/NXBombItem.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

ANXBombItem::ANXBombItem()
{
   
    ExplosionDelay = 3.0f;
    ExplosionRadius = 250.f;
    ExplosionDamage = 30.f;
    ItemType = "Mine";


    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(Scene);  

   
    ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
    ExplosionCollision->InitSphereRadius(ExplosionRadius);
    ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    ExplosionCollision->SetupAttachment(Scene);


   //아이템 던지기
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = Scene;
    ProjectileMovement->InitialSpeed = 100.f;
    ProjectileMovement->MaxSpeed = 100.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.f;
   
}

void ANXBombItem::ThrowItem(const FVector& LaunchVelocity)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = LaunchVelocity;
    }
}

void ANXBombItem::ActivateItem(AActor* Activator)
{

    GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle, this, &ANXBombItem::Explode, ExplosionDelay, false);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
            FString::Printf(TEXT("Mine activated! Exploding in %f seconds"), ExplosionDelay));
    }
}

void ANXBombItem::Explode()
{
    TArray<AActor*> OverlappingActors;
    ExplosionCollision->GetOverlappingActors(OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor->ActorHasTag("NonPlayer"))
        {
          
            UGameplayStatics::ApplyDamage(
                Actor,
                ExplosionDamage,
                nullptr,
                this,
                UDamageType::StaticClass()
            );

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
                    FString::Printf(TEXT("NonPlayer damaged %d by MineItem"), (int)ExplosionDamage));
            }
        }
    }
    DestroyItem();
}