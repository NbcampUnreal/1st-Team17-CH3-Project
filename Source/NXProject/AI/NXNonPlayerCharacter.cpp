#include "AI/NXNonPlayerCharacter.h"
#include "AI/NXAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Animation/NXAIAnimInstance.h"
#include "Engine/OverlapResult.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"		//UProgressBar����� ���� �߰�


ANXNonPlayerCharacter::ANXNonPlayerCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//ANXNonPlayerCharacter�� ������ ��ġ�ǰų� ���Ӱ� �����Ǹ� NXAIController�� ���ǰ� �ڵ����� �����.

//----------���� �ʱ�ȭ----------//
	Defense = 0.f;
	Strength = 0.f;
	MaxHealth = 0.0f;
//-------------------------------//

	AINameplateWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AINameplateWidget"));
	AINameplateWidget->SetupAttachment(GetMesh());
	AINameplateWidget->SetWidgetSpace(EWidgetSpace::World);

	AIHealtBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("AIHealtBar"));
	AIHealtBar->SetupAttachment(GetMesh());
	AIHealtBar->SetWidgetSpace(EWidgetSpace::World);

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
		
		UpdateAINameplate();
		UpdateAIHealtBar();
	}
}

void ANXNonPlayerCharacter::OnCheckHit()
{
	TArray<FOverlapResult> OverlapResults; // �浹 ���� �� ������ ���͵��� ��� ���� �迭
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, this); // �浹 ������ �ʿ��� ���� ����
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults, GetActorLocation(), FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(300.f), CollisionQueryParams
	); // �浹 ���� �Լ� ȣ��

	if (bResult) // �浹 ������ �����ϸ�
	{
		if (ZombieSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				ZombieSound,
				GetActorLocation()
			);
		}

		ACharacter* PlayerCharacter = nullptr; // ù ��° ������ �÷��̾� ĳ���� ���� ����

		for (auto const& OverlapResult : OverlapResults) // �浹 ������ ���͵��� ��ȸ
		{
			PlayerCharacter = Cast<ACharacter>(OverlapResult.GetActor());
			if (IsValid(PlayerCharacter)) // �÷��̾� ĳ���Ͱ� ��ȿ�ϸ�
			{
				// ù ��° �÷��̾ ���� �� �ݺ��� ����
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
	UKismetSystemLibrary::PrintString(this, TEXT("ISDead()"));
	BeginDead();
}

void ANXNonPlayerCharacter::BeginDead()
{
	UNXAIAnimInstance* AnimInstance = Cast<UNXAIAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Stop(0.2f, AttackMontage);
		bIsNowAttacking = false;  // ���� ���� ����
	}
	if (IsValid(AnimInstance) == true && IsValid(DeadMontage) == true && AnimInstance->Montage_IsPlaying(DeadMontage) == false)
	{
		AnimInstance->Montage_Play(DeadMontage);


		if (OnDeadMontageEndedDelegate.IsBound() == false)
		{
			OnDeadMontageEndedDelegate.BindUObject(this, &ThisClass::EndDead);
			AnimInstance->Montage_SetEndDelegate(OnDeadMontageEndedDelegate, DeadMontage);
		}
	}
}

void ANXNonPlayerCharacter::EndDead(UAnimMontage* InMontage, bool bInterruped)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


	GetWorld()->GetTimerManager().ClearTimer(DeadTimerHandle);		// Ÿ�̸� ����

	if (OnDeadMontageEndedDelegate.IsBound() == true)
	{
		OnDeadMontageEndedDelegate.Unbind();
	}
	Destroy();
}

void ANXNonPlayerCharacter::BeginAttack()
{
	UNXAIAnimInstance* AnimInstance = Cast<UNXAIAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	if (Health <= KINDA_SMALL_NUMBER)
	{
		return;
	}

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

	GetWorld()->GetTimerManager().ClearTimer(AttackDamageTimerHandle);		// Ÿ�̸� ����

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
	UE_LOG(LogTemp, Warning, TEXT("Health wdecreased to: %f"), Health);

	if (Health <= 0.0f)
	{
		IsDead();
	}

	UpdateAIHealtBar();
	return ActualDamage;
}

void ANXNonPlayerCharacter::UpdateAINameplate()
{
	if (!AINameplateWidget) return;

	UUserWidget* AINameplateWidgetInstance = AINameplateWidget->GetUserWidgetObject();
	if (!AINameplateWidgetInstance) return;

	//FString Name = GetName();
	FString Name;

	if(MaxHealth > 500)
	{
		Name = (TEXT("aaa"));
		Defense = 3.f;
		Strength = 5.f;
	}
	else if (MaxHealth <= 300)
	{
		Name = (TEXT("bbb"));
		Defense = 6.f;
		Strength = 10.f;
	}
	else
	{
		Name = (TEXT("ccc"));
		Defense = 9.f;
		Strength = 15.f;
	}
	if (UTextBlock* NameText = Cast<UTextBlock>(AINameplateWidgetInstance->GetWidgetFromName(TEXT("AINameplate"))))
	{
		NameText->SetText(FText::FromString(Name));
		//NameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *Name)));
	}
}

void ANXNonPlayerCharacter::UpdateAIHealtBar()
{
	if (!AIHealtBar) return;

	UUserWidget* AIHealtBarWidgetInstance = AIHealtBar->GetUserWidgetObject();
	if (!AIHealtBarWidgetInstance) return;

	if (UProgressBar* AIHPBar = Cast<UProgressBar>(AIHealtBarWidgetInstance->GetWidgetFromName(TEXT("AIHealtBar"))))
	{

		float HPPercent = 0.f;
		if (MaxHealth > 0.f)
		{
			HPPercent = Health / MaxHealth;
		}

		AIHPBar->SetPercent(HPPercent);

		if (HPPercent < 0.3f)
		{
			FLinearColor LowHPColor = FLinearColor::Red;
			AIHPBar->SetFillColorAndOpacity(LowHPColor);
		}
	}
}
#include "GameFramework/CharacterMovementComponent.h"

void ANXNonPlayerCharacter::FreezeMovement()
{
	if (GetCharacterMovement())
	{

		GetCharacterMovement()->DisableMovement();
	}
}

void ANXNonPlayerCharacter::UnfreezeMovement()
{
	if (GetCharacterMovement())
	{
		
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}
