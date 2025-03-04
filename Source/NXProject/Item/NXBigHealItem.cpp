#include "Item/NXBigHealItem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/Engine.h"
#include "Player/NXPlayerCharacter.h"
#include "Kismet/GameplayStatics.h" 
#include "TimerManager.h"

ANXBigHealItem::ANXBigHealItem()
{
    HealAmount = 30.0f;
    ItemType = "BigHeal";
}

void ANXBigHealItem::ActivateItem(AActor* Activator)
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
