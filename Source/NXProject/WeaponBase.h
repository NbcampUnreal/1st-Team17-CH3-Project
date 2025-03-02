#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class ANXProjectile;

UENUM(BlueprintType)
enum class EFireMode : uint8
{
    None  UMETA(DisplayName ="None"),
    SemiAutoUMETA UMETA(DisplayName = "None"),
    Automatic UMETA(DisplayName = "Automatic")
};

UCLASS()
class NXPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AWeaponBase();

	virtual void BeginPlay() override;

    // 총의 스켈레탈 메시 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;

    // 무기의 기본 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    EFireMode FireMode;  // 발사 방식 (히트스캔, 물리탄환)


    

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MaxAmmo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 CurrentAmmo;




    
    // 무기 발사 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

    // 재장전 함수
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Reload();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ReloadTime;

    bool bIsReloading;  //장전 중인지 여부!

private:
    bool bCanFire;
    
    void FinishReload();

    FTimerHandle FireTimerHandle;
};
