#include "Item/NXSmallHealItem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"
#include "Player/NXPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ANXSmallHealItem::ANXSmallHealItem()
{
    HealAmount = 10.0f;
    ItemType = "SmallHeal";
}

void ANXSmallHealItem::ActivateItem(AActor* Activator)
{

    ANXPlayerCharacter* Character = Cast<ANXPlayerCharacter>(Activator);
    if (Character)
    {

        Character->AddHealth(HealAmount);


        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                2.0f,
                FColor::Green,
                TEXT("Healed!")
            );
        }
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
    Destroy();
}
