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

    // �浹 ���� �Լ�
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
    // ��Ż�� �浹 ���� (�ڽ� ������Ʈ)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    UBoxComponent* PortalTrigger;

    // ��Ż�� �ð��� ǥ�� (����ƽ �޽� ������Ʈ)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
    UStaticMeshComponent* PortalMesh;

    // �̵��� ���� �̸� (���� ��ȹ�ڰ� �ν��Ͻ����� ���� ����)
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Portal")
    FName NextLevelName;
};
