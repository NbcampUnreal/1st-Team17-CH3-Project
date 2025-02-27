#include "NXKeyItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/NXPlayerCharacter.h"

ANXKeyItem::ANXKeyItem()
{
    PrimaryActorTick.bCanEverTick = false;


    KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
    RootComponent = KeyMesh;

    
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(KeyMesh);
    CollisionComponent->SetSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));


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
    ANXPlayerCharacter* Character = Cast<ANXPlayerCharacter>(OtherActor);
    if (Character)
    {
        Character->PickupKey();
       
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 2.0f, FColor::Green, TEXT(" Key Acquired!!")
            );
        }

        Destroy();
    }
}
