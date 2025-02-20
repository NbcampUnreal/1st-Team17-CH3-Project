#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXItemInterface.h"
#include "NXBaseItem.generated.h"

UCLASS(Abstract)
class NXPROJECT_API ANXBaseItem : public AActor, public INXItemInterface
{
    GENERATED_BODY()

public:
    ANXBaseItem();

    virtual void OnItemOverlap(AActor* OverlappedActor, AActor* OtherActor) override;
    virtual void OnItemEndOverlap(AActor* OverlappedActor) override;
    virtual void ActivateItem(AActor* Activator) override;
    virtual FName GetItemType() const override;

    UFUNCTION()
    virtual void DestroyItem();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemType;

    UFUNCTION()
    virtual void HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    virtual void HandleEndOverlap(AActor* OverlappedActor);
};
