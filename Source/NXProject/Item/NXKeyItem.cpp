#include "NXKeyItem.h"
#include "WJH_Character.h"
#include "Components/SphereComponent.h"

ANXKeyItem::ANXKeyItem()
{
    PrimaryActorTick.bCanEverTick = false;

    USphereComponent* CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->SetSphereRadius(50.0f);

    // 올바른 델리게이트 바인딩
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANXKeyItem::OnOverlap);
}

void ANXKeyItem::BeginPlay()
{
    Super::BeginPlay();
}

void ANXKeyItem::OnOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!OtherActor) return;

    AWJH_Character* Character = Cast<AWJH_Character>(OtherActor);
    if (Character)
    {
        Character->SetHasKey(true);
        Destroy();
    }
}
