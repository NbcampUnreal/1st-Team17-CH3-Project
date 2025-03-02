#include "WeaponBase.h"
#include "Engine/World.h"
#include "NXProjectile.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
   

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;  // WeaponMesh�� ��Ʈ ������Ʈ�� ����
    FireRate = 0.2f; //�߻� ����
    MaxAmmo = 30;
    ReloadTime = 2.0f; //���� �ð�
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

        //�Ѿ� �߻� �κ�
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



