#include "Item/NXSmallHealItem.h"
#include "NiagaraComponent.h"

ANXSmallHealItem::ANXSmallHealItem()
{
    HealAmount = 10.0f;
    ItemType = "SmallHeal";
}

void ANXSmallHealItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Overlap!!"));

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
