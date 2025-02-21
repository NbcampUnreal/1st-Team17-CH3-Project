#include "Item/NXSmallHealItem.h"

ANXSmallHealItem::ANXSmallHealItem()
{
    HealAmount = 10.0f;
    ItemType = "SmallHeal";
}

void ANXSmallHealItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);
}
