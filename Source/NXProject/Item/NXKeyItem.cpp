
#include "Item/NXKeyItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Engine/Engine.h"

ANXKeyItem::ANXKeyItem()
{
	ItemType = "Key";
}

void ANXKeyItem::ActivateItem(AActor* Activator)
{
	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(Activator);
	if (Player)
	{
		//Player-> bHasKey = true; (캐릭터.cpp 수정후 사용)

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Key Acquired!"));

		}
	}
	DestroyItem();
}

