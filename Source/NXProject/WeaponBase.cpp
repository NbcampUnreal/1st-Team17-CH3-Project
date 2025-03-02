#include "WeaponBase.h"
#include "Engine/World.h"
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
       
}

void AWeaponBase::Reload()
{
}

void AWeaponBase::FinishReload()
{
   
}



