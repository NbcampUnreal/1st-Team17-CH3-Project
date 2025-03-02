#include "WeaponBase.h"
#include "Engine/World.h"
#include "NXProjectile.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
   

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;  // WeaponMesh를 루트 컴포넌트로 설정
    FireRate = 0.2f; //발사 간격
    MaxAmmo = 30;
    ReloadTime = 2.0f; //장전 시간
    bIsReloading = false; 
    bCanFire = true;
    CurrentAmmo = MaxAmmo;
   


}


void AWeaponBase::BeginPlay()
{
   
	Super::BeginPlay();
	
}

void AWeaponBase::Fire()
{
    if (bCanFire && ProjectileClass)

    {
        FVector MuzzzleLocation = GetActorLocation();
        FRotator MuzzleRotation = GetActorRotation();

        //총알 발사 부분
        ANXProjectile* Projectile = GetWorld()->SpawnActor<ANXProjectile>(ProjectileClass, MuzzzleLocation, MuzzleRotation);

        if (Projectile)
        {
            Projectile->FireDirection(MuzzleRotation.Vector());
        }

        CurrentAmmo--;
    }

    else if(CurrentAmmo<=0&& !bIsReloading)
    {
        Reload();
    }

  

    
}

void AWeaponBase::Reload()
{
    if (bIsReloading || CurrentAmmo == MaxAmmo)
    {
        return;
    }

    bIsReloading = true;
   

    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AWeaponBase::FinishReload, ReloadTime, false);

   
    
}

void AWeaponBase::FinishReload()
{
    CurrentAmmo = MaxAmmo;
    bIsReloading = false;

}



