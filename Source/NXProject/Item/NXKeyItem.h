
#pragma once

#include "CoreMinimal.h"
#include "Item/NXBaseItem.h"
#include "NXKeyItem.generated.h"

UCLASS()
class NXPROJECT_API ANXKeyItem : public ANXBaseItem
{
	GENERATED_BODY()
	

public:
	ANXKeyItem();

protected:

	virtual void ActivateItem(AActor* Activator) override;
};
