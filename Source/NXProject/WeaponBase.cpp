#include "WeaponBase.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
   
    FireMode = EFireMode::HitScan;
    DamageAmount = 25.f;
    FireRate = 0.1f;
    MaxAmmo = 30;
    ReloadTime = 2.f;
    CurrentAmmo = MaxAmmo;
    bIsReloading = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;  // WeaponMesh를 루트 컴포넌트로 설정
    

}


void AWeaponBase::BeginPlay()
{
   
	Super::BeginPlay();
	
}

void AWeaponBase::Fire()
{

    if (bIsReloading || CurrentAmmo <= 0)
    {
        return;  // 재장전 중이거나 장탄수가 없다면 발사하지 않음
    }

    if (FireMode == EFireMode::HitScan)
    {
        FVector Start = GetActorLocation();  // 발사 지점
        FVector Forward = GetActorForwardVector();  // 발사 방향
        FVector End = Start + (Forward * 10000.f);  // 발사 거리 설정 (예: 10000 units)

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;

        // 레이캐스트 발사
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
        {
            // 목표물에 맞았을 때 처리
            if (AActor* HitActor = HitResult.GetActor())
            {
                // 데미지 처리 (예: 데미지 입히기)
                UGameplayStatics::ApplyPointDamage(HitActor, DamageAmount, Forward, HitResult, nullptr, this, nullptr);
               
            }
        }
    }
    else if (FireMode == EFireMode::Projectile)
    {
        // 물리탄환 방식 처리
    }

    CurrentAmmo--;
}

void AWeaponBase::Reload()
{

    if (bIsReloading || CurrentAmmo == MaxAmmo)
    {
        return;  // 이미 재장전 중이거나 장탄수가 가득 차면 재장전하지 않음
    }

    bIsReloading = true;

    // 일정 시간 후 재장전 완료 처리 (여기서는 단순히 시간 지연을 사용)
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            CurrentAmmo = MaxAmmo;  // 장탄수 복원
            bIsReloading = false;  // 재장전 완료
        }, ReloadTime, false);
}



