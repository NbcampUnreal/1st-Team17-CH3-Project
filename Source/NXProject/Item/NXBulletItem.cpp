
#include "Item/NXBulletItem.h"
#include "Player/NXPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
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
	
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange, TEXT("Bullet Refilled!"));
		}
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				PickupSound,
				GetActorLocation()
			);
		}
	}
	DestroyItem();
}
