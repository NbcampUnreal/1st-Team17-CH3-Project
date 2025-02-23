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
	//ANXNonPlayerCharacter�� ������ ��ġ�ǰų� ���Ӱ� �����Ǹ� NXAIController�� ���ǰ� �ڵ����� �����.

//----------���� �ʱ�ȭ----------//
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
//		GetWorld()->GetTimerManager().SetTimer(		// Ÿ�̸� ���� (0.5�� �� ������ ����)
//			AttackDamageTimerHandle,
//			this,
//			&ANXNonPlayerCharacter::ApplyAttackDamage,
//			0.5f, // ������ ���� �ð�
//			false // �ݺ� ����
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
//	GetWorld()->GetTimerManager().ClearTimer(AttackDamageTimerHandle);		// Ÿ�̸� ����
//
//	if (OnAttackMontageEndedDelegate.IsBound() == true)
//	{
//		OnAttackMontageEndedDelegate.Unbind();
//	}
//}
//
//void ANXNonPlayerCharacter::ApplyAttackDamage()
//{
//	// �ֺ� ���� �˻�
//	TArray<AActor*> OverlappingActors;
//	GetOverlappingActors(OverlappingActors);
//
//	for (AActor* Actor : OverlappingActors)
//	{
//		if (Actor && Actor->ActorHasTag("Player"))
//		{
//			// ������ ����
//			UGameplayStatics::ApplyDamage(
//				Actor,
//				Strength, // Strength�� ĳ������ ���ݷ�
//				GetController(),
//				this,
//				UDamageType::StaticClass()
//			);
//		}
//	}
//}

float ANXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ������ ����� ���� ������ ���
	float FinalDamage = FMath::Max(0.f, DamageAmount - Defense);
	CurrentHP = FMath::Clamp(CurrentHP - FinalDamage, 0.f, MaxHP);

	// ü���� 0 �������� Ȯ��
	if (CurrentHP <= 0.f || KINDA_SMALL_NUMBER)
	{
		// AI ���� �� ĳ���� �ı�
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
	// ü���� 0 �������� Ȯ��
	return CurrentHP <= 0.f;
}