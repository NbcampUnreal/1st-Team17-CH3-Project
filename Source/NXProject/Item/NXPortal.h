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

    // 충돌 이벤트
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

    // 이동할 레벨 이름 (우창님이 변경)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Portal")
    FName NextLevelName;
};
