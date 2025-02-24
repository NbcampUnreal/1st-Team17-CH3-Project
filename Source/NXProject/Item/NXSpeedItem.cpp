
#include "Item/NXSpeedItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Engine/Engine.h"

ANXSpeedItem::ANXSpeedItem()
{
	SpeedBoostAmount = 150.f;
	BoostDuration = 10.f;
	ItemType = "Speed";
}

void ANXSpeedItem::ActivateItem(AActor* Activator)
{
	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(Activator);
	if (Player)
	{
		//Player->IncreaseSpeed(SpeedBoostAmount, BoostDuration); 
		//Character.cpp 수정후 주석 해제 에정
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Speed Boost Activated!"));
		}
	}
	DestroyItem();
}