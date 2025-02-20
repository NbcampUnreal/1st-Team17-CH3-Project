#include "NXBaseItem.h"

ANXBaseItem::ANXBaseItem()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANXBaseItem::HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    // 필요에 따라 기능 구현 추가
}

void ANXBaseItem::HandleEndOverlap(AActor* OverlappedActor)
{
   
}

void ANXBaseItem::OnItemOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
 
}

void ANXBaseItem::OnItemEndOverlap(AActor* OverlappedActor)
{
   
}

void ANXBaseItem::ActivateItem(AActor* Activator)
{
    
}

FName ANXBaseItem::GetItemType() const
{
    return ItemType;
}

void ANXBaseItem::DestroyItem()
{
    Destroy();
}
