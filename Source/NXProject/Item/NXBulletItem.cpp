
#include "Item/NXBulletItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Engine/Engine.h"

ANXBulletItem::ANXBulletItem()
{
	BulletAmount = 10;
	ItemType = "Bullet";
}

void ANXBulletItem::ActivateItem(AActor* Activator)
{
	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(Activator);
	if (Player)
	{
		//Player->RifillBullet(BulletAmount);
		//Character.cpp 수정 후 주석 해제

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Bullet Refilled!"));
		}
	}
	DestroyItem();
}
