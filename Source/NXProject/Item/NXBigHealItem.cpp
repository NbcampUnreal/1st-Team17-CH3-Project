
#include "Item/NXBigHealItem.h"

ANXBigHealItem::ANXBigHealItem()
{
    HealAmount = 30.0f;
    ItemType = "BigHeal";
}

void ANXBigHealItem::ActivateItem(AActor* Activator)
{
    DestroyItem();  
}
