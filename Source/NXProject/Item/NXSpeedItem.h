
#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXSpeedItem.generated.h"


UCLASS()
class NXPROJECT_API ANXSpeedItem : public ANXBaseItem
{
	GENERATED_BODY()

public:
	ANXSpeedItem();

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item")
	float SpeedBoostAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float BoostDuration;
	
	virtual void ActivateItem(AActor* Activator)override;
};
