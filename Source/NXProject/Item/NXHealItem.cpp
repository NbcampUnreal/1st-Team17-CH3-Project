
#include "Item/NXHealItem.h"

ANXHealItem::ANXHealItem()
{
    HealAmount = 0.0f;
}

void ANXHealItem::ActivateItem(AActor* Activator)
{
   
    if (GEngine)
    {
        FString DebugMsg = FString::Printf(TEXT("Healing Amount: %f"), HealAmount);
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, DebugMsg);
    }

   
    DestroyItem();
}
