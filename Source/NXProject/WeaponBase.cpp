#include "WeaponBase.h"
#include "Engine/World.h"
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
       
}

void AWeaponBase::Reload()
{
}

void AWeaponBase::FinishReload()
{
   
}



