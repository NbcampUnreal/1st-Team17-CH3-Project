
#include "Item/NXKeyItem.h"
#include "Engine/Engine.h"

ANXKeyItem::ANXKeyItem()
{
	ItemType = "Key";
}

void ANXKeyItem::ActivateItem(AActor* Activator)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Key Acquired!"));

	}

	DestroyItem();
}

