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

    // ������ �⺻ �Ӽ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    EFireMode FireMode;  // �߻� ��� (��Ʈ��ĵ, ����źȯ)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float DamageAmount;  // ��������

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float FireRate;  // �߻� �ӵ�

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    int32 MaxAmmo;  // ��ź��

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stats")
    float ReloadTime;  // ������ �ð�

    // ���� �߻� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
    void Fire();

    // ������ �Լ�
    UFUNCTION(BlueprintCallable, Category = "Weapon Functions")
    void Reload();

    int32 CurrentAmmo;  // ���� ��ź��

private:
    bool bIsReloading;  // ������ �� ����
};
