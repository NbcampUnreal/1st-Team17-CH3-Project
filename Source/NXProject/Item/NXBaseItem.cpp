#include "NXBaseItem.h"

ANXBaseItem::ANXBaseItem()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANXBaseItem::HandleBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
    // �ʿ信 ���� ��� ���� �߰�
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
