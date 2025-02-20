
#include "Item/NXMineItem.h"

ANXMineItem::ANXMineItem()
{
	ExplosionDelay = 0.5f;
	ExplosionRadius = 250;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
}

void ANXMineItem::ActivateItem(AActor* Activator)
{
	DestroyItem();
}
