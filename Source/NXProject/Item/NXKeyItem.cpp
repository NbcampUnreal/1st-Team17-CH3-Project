#include "NXKeyItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
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
                -1,
                2.0f,
                FColor::Green,
                TEXT("Key Acquired!!")
            );
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
}

void ANXKeyItem::ActivateItem(AActor* Activator)
{
   
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Overlap!!"));
    }

   
}
