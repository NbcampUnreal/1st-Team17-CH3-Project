#include "NXBaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Particles/ParticleSystemComponent.h"

ANXBaseItem::ANXBaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);


    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    Collision->SetupAttachment(Scene);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ANXBaseItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ANXBaseItem::OnItemEndOverlap);

}

void ANXBaseItem::OnItemOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!!")));
		
		ActivateItem(OtherActor);
	}
}

void ANXBaseItem::OnItemEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ANXBaseItem::ActivateItem(AActor* Activator)
{

}

FName ANXBaseItem::GetItemType() const
{
	return ItemType;
}

void ANXBaseItem::DestroyItem()
{
	Destroy();
}