#include "NXPortal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ANXPortal::ANXPortal()
{
    PrimaryActorTick.bCanEverTick = false;

    // 박스 컴포넌트 생성 및 설정
    PortalTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PortalTrigger"));
    RootComponent = PortalTrigger;
    PortalTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 150.0f));

    // 충돌 설정: Pawn(플레이어)와만 Overlap 발생
    PortalTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PortalTrigger->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    PortalTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    PortalTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PortalTrigger->SetGenerateOverlapEvents(true);

    // 오버랩 이벤트 바인딩
    PortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANXPortal::OnOverlap);
}

void ANXPortal::BeginPlay()
{
    Super::BeginPlay();

    // 디버그 로그: 설정된 NextLevelName 확인
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
    if (!OtherActor)
        return;

    UE_LOG(LogTemp, Warning, TEXT("NXPortal: Overlap detected with actor: %s"), *OtherActor->GetName());

    if (OtherActor->ActorHasTag("Player"))
    {
        UE_LOG(LogTemp, Warning, TEXT("NXPortal: Player detected. Moving to level: %s"), *NextLevelName.ToString());
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("NXPortal Activated!"));
        }
        if (!NextLevelName.IsNone())
        {
            UGameplayStatics::OpenLevel(this, NextLevelName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("NXPortal Error: NextLevelName is not set!"));
        }
    }
}
