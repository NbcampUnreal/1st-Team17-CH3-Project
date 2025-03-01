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
    RootComponent = WeaponMesh;  // WeaponMesh�� ��Ʈ ������Ʈ�� ����
    

}


void AWeaponBase::BeginPlay()
{
   
	Super::BeginPlay();
	
}

void AWeaponBase::Fire()
{

    if (bIsReloading || CurrentAmmo <= 0)
    {
        return;  // ������ ���̰ų� ��ź���� ���ٸ� �߻����� ����
    }

    if (FireMode == EFireMode::HitScan)
    {
        FVector Start = GetActorLocation();  // �߻� ����
        FVector Forward = GetActorForwardVector();  // �߻� ����
        FVector End = Start + (Forward * 10000.f);  // �߻� �Ÿ� ���� (��: 10000 units)

        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;

        // ����ĳ��Ʈ �߻�
        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
        {
            // ��ǥ���� �¾��� �� ó��
            if (AActor* HitActor = HitResult.GetActor())
            {
                // ������ ó�� (��: ������ ������)
                UGameplayStatics::ApplyPointDamage(HitActor, DamageAmount, Forward, HitResult, nullptr, this, nullptr);
               
            }
        }
    }
    else if (FireMode == EFireMode::Projectile)
    {
        // ����źȯ ��� ó��
    }

    CurrentAmmo--;
}

void AWeaponBase::Reload()
{

    if (bIsReloading || CurrentAmmo == MaxAmmo)
    {
        return;  // �̹� ������ ���̰ų� ��ź���� ���� ���� ���������� ����
    }

    bIsReloading = true;

    // ���� �ð� �� ������ �Ϸ� ó�� (���⼭�� �ܼ��� �ð� ������ ���)
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            CurrentAmmo = MaxAmmo;  // ��ź�� ����
            bIsReloading = false;  // ������ �Ϸ�
        }, ReloadTime, false);
}



