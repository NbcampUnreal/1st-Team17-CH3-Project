#include "NXArmorItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ANXArmorItem::ANXArmorItem()
{
    ArmorValue = 20.0f;
    ArmorDuration = 10.0f;


    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;


    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(SceneRoot);
    CollisionComponent->SetSphereRadius(50.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ANXArmorItem::OnOverlap);


    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(SceneRoot);

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANXArmorItem::BeginPlay()
{
    Super::BeginPlay();
}

void ANXArmorItem::OnOverlap(
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
        
        Character->IncreaseArmor(ArmorValue, ArmorDuration);
      

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                2.0f,
                FColor::Blue,
                FString::Printf(TEXT("Armor increased by %f "), ArmorValue, ArmorDuration)
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
