#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXPortal.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class NXPROJECT_API ANXPortal : public AActor
{
    GENERATED_BODY()

public:
    ANXPortal();

protected:
    virtual void BeginPlay() override;

    // 충돌 감지 함수
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
    // 포탈의 충돌 영역 (박스 컴포넌트)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    UBoxComponent* PortalTrigger;

    // 포탈의 시각적 표현 (스태틱 메쉬 컴포넌트)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    UStaticMeshComponent* PortalMesh;

    // 이동할 레벨 이름 (레벨 기획자가 인스턴스별로 설정 가능)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Portal")
    FName NextLevelName;
};
