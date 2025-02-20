
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NXItemInterface.h"
#include "NXBaseItem.generated.h"

UCLASS()
class NXPROJECT_API ANXBaseItem : public AActor, public INXItemInterface
{
	GENERATED_BODY()
	
public:	

	ANXBaseItem();

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")

	FName ItemType;

	virtual void OnItemOverlap(AActor* OverlapActor) override;
	virtual void OnItemEndOverlap(AActor* OverlapActor) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	virtual void DestroyItem();



};
