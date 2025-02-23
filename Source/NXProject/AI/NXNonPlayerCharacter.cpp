#include "AI/NXNonPlayerCharacter.h"

#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "Animation/NXAnimInstance.h"

ANXNonPlayerCharacter::ANXNonPlayerCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//ANXNonPlayerCharacter는 레벨에 배치되거나 새롭게 생성되면 NXAIController의 빙의가 자동으로 진행됨.

//----------스텟 초기화----------//
	MaxHP = 100.f;
	CurrentHP = MaxHP;
	Defense = 10.f;
	Strength = 15.f;
	//-------------------------------//
}

void ANXNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		CurrentHP = MaxHP;

		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

//void ANonPlayerCharacter::BeginAttack()
//{
//	UNXAnimInstance* AnimInstance = Cast<UNXAnimInstance>(GetMesh()->GetAnimInstance());
//	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));
//
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
//	if (IsValid(AnimInstance) == true && IsValid(AttackMontage) == true && AnimInstance->Montage_IsPlaying(AttackMontage) == false)
//	{
//		AnimInstance->Montage_Play(AttackMontage);
//
//		bIsNowAttacking = true;
//
//		GetWorld()->GetTimerManager().SetTimer(		// 타이머 설정 (0.5초 후 데미지 적용)
//			AttackDamageTimerHandle,
//			this,
//			&ANXNonPlayerCharacter::ApplyAttackDamage,
//			0.5f, // 데미지 적용 시간
//			false // 반복 없음
//		);
//
//		if (OnAttackMontageEndedDelegate.IsBound() == false)
//		{
//			OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
//			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMontage);
//		}
//	}
//}
//
//void ANXNonPlayerCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
//{
//	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
//
//	bIsNowAttacking = false;
//
//	GetWorld()->GetTimerManager().ClearTimer(AttackDamageTimerHandle);		// 타이머 해제
//
//	if (OnAttackMontageEndedDelegate.IsBound() == true)
//	{
//		OnAttackMontageEndedDelegate.Unbind();
//	}
//}
//
//void ANXNonPlayerCharacter::ApplyAttackDamage()
//{
//	// 주변 액터 검색
//	TArray<AActor*> OverlappingActors;
//	GetOverlappingActors(OverlappingActors);
//
//	for (AActor* Actor : OverlappingActors)
//	{
//		if (Actor && Actor->ActorHasTag("Player"))
//		{
//			// 데미지 적용
//			UGameplayStatics::ApplyDamage(
//				Actor,
//				Strength, // Strength는 캐릭터의 공격력
//				GetController(),
//				this,
//				UDamageType::StaticClass()
//			);
//		}
//	}
//}

float ANXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 방어력을 고려한 최종 데미지 계산
	float FinalDamage = FMath::Max(0.f, DamageAmount - Defense);
	CurrentHP = FMath::Clamp(CurrentHP - FinalDamage, 0.f, MaxHP);

	// 체력이 0 이하인지 확인
	if (CurrentHP <= 0.f || KINDA_SMALL_NUMBER)
	{
		// AI 종료 및 캐릭터 파괴
		ANXAIController* AIController = Cast<ANXAIController>(GetController());
		if (IsValid(AIController))
		{
			AIController->EndAI();
		}
		Destroy();
	}

	return FinalDamage;
}

bool ANXNonPlayerCharacter::IsDead() const
{
	// 체력이 0 이하인지 확인
	return CurrentHP <= 0.f;
}