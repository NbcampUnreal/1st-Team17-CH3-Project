#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "NXItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UNXItemInterface : public UInterface
{
    GENERATED_BODY()
};

class NXPROJECT_API INXItemInterface
{
    GENERATED_BODY()

public:
    virtual void OnItemOverlap(AActor* OverlappedActor, AActor* OtherActor) = 0;
    virtual void OnItemEndOverlap(AActor* OverlappedActor) = 0;
    virtual void ActivateItem(AActor* Activator) = 0;
    virtual FName GetItemType() const = 0;
};
