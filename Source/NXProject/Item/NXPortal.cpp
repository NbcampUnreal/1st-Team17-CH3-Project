#include "NXPortal.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ANXPortal::ANXPortal()
{
    PrimaryActorTick.bCanEverTick = false;

    
    PortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalTrigger"));
    RootComponent = PortalTrigger;

    PortalTrigger->SetBoxExtent(FVector(100.f, 100.f, 150.f));
    PortalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PortalTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    PortalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    PortalTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PortalTrigger->SetGenerateOverlapEvents(true);


    PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXPortal::OnOverlap);


    NextLevelName = NAME_None;
    PickupNiagara = nullptr;
    bActivated = false;
}

void ANXPortal::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("NXPortal Initialized. Next Level: %s"), *NextLevelName.ToString());
}

void ANXPortal::OnOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!OtherActor) return;

    UE_LOG(LogTemp, Warning, TEXT("NXPortal: Overlap detected with actor: %s"), *OtherActor->GetName());

    if (!bActivated && OtherActor->ActorHasTag("Player"))
    {
        bActivated = true; 

        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                GetWorld(),
                PickupSound,
                GetActorLocation()
            );
        }

        UE_LOG(LogTemp, Warning, TEXT("NXPortal: Player detected. Will move to level: %s in 1 second."), *NextLevelName.ToString());

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NXPortal Activated!"));
        }

        if (!NextLevelName.IsNone())
        {
           
            TWeakObjectPtr<ANXPortal> WeakPortal(this);
            FTimerHandle TimerHandle;
            FTimerDelegate TimerDelegate;
            TimerDelegate.BindLambda([WeakPortal]()
                {
                    if (WeakPortal.IsValid())
                    {
                        WeakPortal->MoveToNextLevel();
                    }
                });
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NXPortal Error: NextLevelName is not set!"));
        }

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
        TWeakObjectPtr<UNiagaraComponent> WeakNiagara(NiagaraComponent);
        FTimerHandle DestroyNiagaraTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            DestroyNiagaraTimerHandle,
            [WeakNiagara]()
            {
                if (WeakNiagara.IsValid())
                {
                    WeakNiagara->DestroyComponent();
                }
            },
            2.0f,
            false
        );
    }

}

void ANXPortal::MoveToNextLevel()
{
    if (!NextLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, NextLevelName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("NXPortal Error: NextLevelName is not set!"));
    }
}
