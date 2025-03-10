
#include "NXSpeedItem.h"
#include "Player/NXPlayerCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ANXSpeedItem::ANXSpeedItem()
{
	SpeedBoostAmount = 1500.f;
	BoostDuration = 10.f;
	ItemType = "Speed";
}

void ANXSpeedItem::ActivateItem(AActor* Activator)
{
	ANXPlayerCharacter* Player = Cast<ANXPlayerCharacter>(Activator);
	if (Player)
	{
		Player->IncreaseSpeed(SpeedBoostAmount, BoostDuration);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Speed Boost Activated!"));
		}


        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                GetWorld(),
                PickupSound,
                GetActorLocation()
            );
        }

        UNiagaraComponent* NiagaraComponent = nullptr;
        if (PickupNiagara)
        {
            NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                PickupNiagara,
                GetActorLocation(),
                GetActorRotation(),
                FVector(1.0f),
                false,
                true,
                ENCPoolMethod::None
            );
        }


        if (NiagaraComponent)
        {
            FTimerHandle DestroyNiagaraTimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                DestroyNiagaraTimerHandle,
                [NiagaraComponent]()
                {
                    NiagaraComponent->DestroyComponent();
                },
                2.0f,
                false
            );
        }
	}
	DestroyItem();
}