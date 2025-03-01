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
#include "Components/ProgressBar.h"		//UProgressBar사용을 위한 추가


ANXNonPlayerCharacter::ANXNonPlayerCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ANXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//ANXNonPlayerCharacter는 레벨에 배치되거나 새롭게 생성되면 NXAIController의 빙의가 자동으로 진행됨.

//----------스텟 초기화----------//
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
	TArray<FOverlapResult> OverlapResults; // 충돌 감지 결과를 저장할 배열
	FCollisionQueryParams CollisionQueryParams(NAME_None, false, this); // 충돌 쿼리 파라미터 설정

	// 다른 AI를 무시하도록 설정
	TArray<AActor*> OtherAIs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANXNonPlayerCharacter::StaticClass(), OtherAIs); // 모든 AI 액터 가져오기
	CollisionQueryParams.AddIgnoredActors(OtherAIs); // 다른 AI들을 충돌 감지에서 제외

	// 구체 형태로 충돌 감지 수행 (ECC_GameTraceChannel2는 플레이어 전용 채널로 가정)
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, // 플레이어 전용 채널 사용
		FCollisionShape::MakeSphere(300.f),
		CollisionQueryParams
	);

	// 충돌 감지에 성공한 경우
	if (bResult)
	{
		// 좀비 사운드 재생
		if (ZombieSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				ZombieSound,
				GetActorLocation()
			);
		}

		// 충돌 감지된 모든 액터 순회
		for (auto const& OverlapResult : OverlapResults)
		{
			// 충돌한 액터가 플레이어 캐릭터인지 확인
			ACharacter* PlayerCharacter = Cast<ACharacter>(OverlapResult.GetActor());
			if (IsValid(PlayerCharacter))
			{
				// 플레이어 컨트롤러인지 추가로 확인
				APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
				if (IsValid(PlayerController))
				{
					// 플레이어에게 데미지 적용
					PlayerCharacter->TakeDamage(Strength, FDamageEvent(), GetController(), this);
					break; // 첫 번째 플레이어만 공격 후 종료
				}
			}
		}
	}

	// 디버그용 구체 및 메시지 출력
	DrawDebugSphere(GetWorld(), GetActorLocation(), 300.f, 16, FColor::Green, false, 5.f);
	UKismetSystemLibrary::PrintString(this, TEXT("OnCheckHit()"));
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
		bIsNowAttacking = false;  // 공격 상태 해제
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


	GetWorld()->GetTimerManager().ClearTimer(DeadTimerHandle);		// 타이머 해제

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
