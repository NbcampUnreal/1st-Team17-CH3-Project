#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXPortal.generated.h"

class UBoxComponent;

UCLASS()
class NXPROJECT_API ANXPortal : public AActor
{
    GENERATED_BODY()

public:
    ANXPortal();

protected:
    virtual void BeginPlay() override;

    // �浹 �̺�Ʈ
    UFUNCTION()
    void OnOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

public:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    UBoxComponent* PortalTrigger;

    // �̵��� ���� �̸� (��â���� ����)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Portal")
    FName NextLevelName;
};
