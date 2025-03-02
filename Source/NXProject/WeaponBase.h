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

    // ���� ���̷�Ż �޽� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USkeletalMeshComponent* WeaponMesh;

    // ������ �⺻ �Ӽ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    EFireMode FireMode;  // �߻� ��� (��Ʈ��ĵ, ����źȯ)


    

    UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
    float FireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 MaxAmmo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    int32 CurrentAmmo;




    
    // ���� �߻� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

    // ������ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Reload();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float ReloadTime;

    bool bIsReloading;  //���� ������ ����!

private:
    bool bCanFire;
    
    void FinishReload();

    FTimerHandle FireTimerHandle;
};
