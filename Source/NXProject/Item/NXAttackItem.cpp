
#include "Item/NXAttackItem.h"
#include "Player/NXPlayerCharacter.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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

