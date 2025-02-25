#include "NXArmorItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
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
        
        // Character->IncreaseArmor(ArmorValue, ArmorDuration);
        // ���� ���� ���� �߰� (Character ���� �Լ��� �ʿ�)
        //Charater.cpp ���� �� �ּ� ����

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                2.0f,
                FColor::Blue,
                FString::Printf(TEXT("Armor increased by %f "), ArmorValue, ArmorDuration)
            );
        }

        
        Destroy();
    }
}
