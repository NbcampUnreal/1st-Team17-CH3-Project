
#include "Item/NXAttackItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Engine/Engine.h"

ANXAttackItem::ANXAttackItem()
{
	AttackBoostAmount = 15.f;
	BoostDuration = 10.f;
	ItemType = "Attack";
}

void ANXAttackItem::ActivateItem(AActor* Activator)
{
	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(Activator);
	if (GEngine)
	{
		Player->IncreaseAttack(AttackBoostAmount, BoostDuration);
	
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Attack Boost Activated!"));
		}
	}
	DestroyItem();
}

