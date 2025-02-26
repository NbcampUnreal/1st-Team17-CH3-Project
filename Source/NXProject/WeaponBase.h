#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EFireMode : uint8
{
    HitScan UMETA(DisplayName = "HitScan"),
    Projectile UMETA(DisplayName = "Projectile")
};

UCLASS()
class NXPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWeaponBase();

	
	virtual void BeginPlay() override;

    // 무기의 기본 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    EFireMode FireMode;  // 발사 방식 (히트스캔, 물리탄환)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float DamageAmount;  // 데미지량

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float FireRate;  // 발사 속도

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    int32 MaxAmmo;  // 장탄수

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float ReloadTime;  // 재장전 시간

    // 무기 발사 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
    void Fire();

    // 재장전 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
    void Reload();

    int32 CurrentAmmo;  // 현재 장탄수

private:
    bool bIsReloading;  // 재장전 중 여부
};
