#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/NXAIAnimInstance.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"


ANXNonPlayerCharacter::ANXNonPlayerCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//ANXNonPlayerCharacter는 레벨에 배치되거나 새롭게 생성되면 NXAIController의 빙의가 자동으로 진행됨.

//----------스텟 초기화----------//
	Defense = 10.f;
	Strength = 15.f;
	MaxHealth = 500.0f;
//-------------------------------//
}

void ANXNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		Health = MaxHealth;

		UNXAIAnimInstance* AIAnimInstance = Cast<UNXAIAnimInstance>(GetMesh()->GetAnimInstance());
		if (IsValid(AIAnimInstance) == true)
		{
			AIAnimInstance->OnCheckHit.AddDynamic(this, &ThisClass::OnCheckHit);
		}

		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void ANXNonPlayerCharacter::OnCheckHit()
{
	TArray<FOverlapResult> OverlapResults; // 충돌 감지 후 감지된 액터들을 담아 놓을 배열
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, this); // 충돌 감지에 필요한 변수 선언
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults, GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(300.f), CollisionQueryParams
	); // 충돌 감지 함수 호출

	if (bResult) // 충돌 감지에 성공하면
	{
		ACharacter* PlayerCharacter = nullptr; // 첫 번째 감지된 플레이어 캐릭터 저장 변수

		for (auto const& OverlapResult : OverlapResults) // 충돌 감지된 액터들을 순회
		{
			PlayerCharacter = Cast<ACharacter>(OverlapResult.GetActor());
			if (IsValid(PlayerCharacter)) // 플레이어 캐릭터가 유효하면
			{
				// 첫 번째 플레이어만 공격 후 반복문 종료
				PlayerCharacter->TakeDamage(Strength, FDamageEvent(), GetController(), this);
				//UE_LOG(LogTemp, Warning, TEXT(" Player Health decreased to: %f"), Health);
				break;
			}
		}
	}

	DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Green, false, 5.f);
	UKismetSystemLibrary::PrintString(this, TEXT("OnCheckHit()"));

	//UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);
}

void ANXNonPlayerCharacter::IsDead()
{
	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("IsDead"));
	
}

void ANXNonPlayerCharacter::BeginAttack()
{
	UNXAIAnimInstance* AnimInstance = Cast<UNXAIAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));



	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
	{
		AnimInstance->Montage_Play(AttackMontage);

		bIsNowAttacking = true;

		if (OnAttackMontageEndedDelegate.IsBound() == false)
		{
			OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMontage);
		}
	}
}

void ANXNonPlayerCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bIsNowAttacking = false;

	GetWorld()->GetTimerManager().ClearTimer(AttackDamageTimerHandle);		// 타이머 해제

	if (OnAttackMontageEndedDelegate.IsBound() == true)
	{
		OnAttackMontageEndedDelegate.Unbind();
	}
}


float ANXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float DamageCalculation(DamageAmount - Defense);
	if (DamageCalculation <= 0)
	{
		DamageCalculation = 0;
	}
	Health = FMath::Clamp(Health - DamageCalculation, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

	if (Health <= 0.0f)
	{
		IsDead();
	}

	return ActualDamage;
}
