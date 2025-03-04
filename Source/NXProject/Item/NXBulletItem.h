
#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXBulletItem.generated.h"

UCLASS()
class NXPROJECT_API ANXBulletItem : public ANXBaseItem
{
	GENERATED_BODY()
	
public:
	ANXBulletItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 BulletAmount;

	virtual void ActivateItem(AActor* Activator)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;

};
