// NXNPCSpawner.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXNPCSpawner.generated.h"

class UBoxComponent;
class ANXNonPlayerCharacter;

UCLASS()
class NXPROJECT_API ANXNPCSpawner : public AActor
{
    GENERATED_BODY()

public:
    ANXNPCSpawner();

protected:
    virtual void BeginPlay() override;


    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);


    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SpawnMultipleNPCs(int32 count);


    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FVector GetRandomSpawnPoint() const;

public:

    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<ANXNonPlayerCharacter> NPCClass;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* NPCTrigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* NPCSpawnVolume;
    UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawning")
    int32 NumberOfNPC = 15;
};
