

#include "NXNPCSpawner.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Player/NXPlayerCharacter.h"
#include "AI/NXNonPlayerCharacter.h"

ANXNPCSpawner::ANXNPCSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    NPCTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("NPCTrigger"));
    NPCTrigger->SetBoxExtent(FVector(200.f, 200.f, 100.f));
    NPCTrigger->SetCollisionProfileName(TEXT("Trigger"));
    NPCTrigger->SetGenerateOverlapEvents(true);
    RootComponent = NPCTrigger;

    NPCTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXNPCSpawner::OnOverlapBegin);


    NPCSpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("NPCSpawnVolume"));
    NPCSpawnVolume->SetBoxExtent(FVector(500.f, 500.f, 200.f));
    NPCSpawnVolume->SetupAttachment(RootComponent);
}

void ANXNPCSpawner::BeginPlay()
{
    Super::BeginPlay();
}

void ANXNPCSpawner::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{

    ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(OtherActor);
    if (Player)
    {
  
        SpawnMultipleNPCs(NumberOfNPC);
    }
}

void ANXNPCSpawner::SpawnMultipleNPCs(int32 Count)
{
    if (!NPCClass)
    {
        return;
    }

    for (int32 i = 0; i < Count; i++)
    {
        FVector SpawnLocation = GetRandomSpawnPoint();
        FRotator SpawnRotation = FRotator::ZeroRotator;

        GetWorld()->SpawnActor<ANXNonPlayerCharacter>(NPCClass, SpawnLocation, SpawnRotation);
    }
}

FVector ANXNPCSpawner::GetRandomSpawnPoint() const
{
    if (NPCSpawnVolume)
    {
        FVector Origin = NPCSpawnVolume->GetComponentLocation();
        FVector Extent = NPCSpawnVolume->GetScaledBoxExtent();

        float RandomX = FMath::FRandRange(-Extent.X, Extent.X);
        float RandomY = FMath::FRandRange(-Extent.Y, Extent.Y);
        float RandomZ = FMath::FRandRange(-Extent.Z, Extent.Z);

        return Origin + FVector(RandomX, RandomY, RandomZ);
    }

    return GetActorLocation();
}
