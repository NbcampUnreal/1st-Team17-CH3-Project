#include "AI/NXAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

//const float ANXAIController::PatrolRepeatInterval(3.f);

const float ANXAIController::PatrolRadius(500.f);
int32 ANXAIController::ShowAIDebug(0);
const FName ANXAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName ANXAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));
const FName ANXAIController::TargetCharacterKey(TEXT("TargetCharacter"));

FAutoConsoleVariableRef CVarShowAIDebug(
	TEXT("NXProject.ShowAIDebug"),
	ANXAIController::ShowAIDebug,
	TEXT(""),
	ECVF_Cheat
);

ANXAIController::ANXAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
	//BrainComponent�� AIController Ŭ������ ���ǵ� �Ӽ�. UBrainComponent <- UBehaviorTreeComponent
}


void ANXAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

	}

	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn) == true)
	{
		BeginAI(ControlledPawn);
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ANXAIController::UpdatePatrolLocation);
		GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &ANXAIController::UpdatePatrolLocation, 5.0f, true); // 5�ʸ��� ���� ��ġ ������Ʈ
	}
}

void ANXAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(InPawn))
	{
		BeginAI(InPawn); // ������ �Ŀ��� AI �ʱ�ȭ ����
	}
}

void ANXAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	EndAI();
}

void ANXAIController::BeginAI(APawn* InPawn)
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent))
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent))
		{
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
			checkf(bRunSucceeded, TEXT("Fail to run behavior tree."));

			// �׺���̼� �ý��� ��������
			UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
			FVector RandomLocation = InPawn->GetActorLocation(); // �⺻�� ����

			if (IsValid(NavSystem))
			{
				// ���� ���� ��ġ ã��
				FNavLocation OutNavLocation;
				bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(InPawn->GetActorLocation(), PatrolRadius, OutNavLocation);
				if (bFoundLocation)
				{
					RandomLocation = OutNavLocation.Location;
				}
			}

			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
			BlackboardComponent->SetValueAsVector(EndPatrolPositionKey, RandomLocation); // ������ �κ�

			if (ShowAIDebug == 1)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI() - ���� ���� ��ġ: %s"), *InPawn->GetActorLocation().ToString()));
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI() - ���� ���� ��ġ: %s"), *RandomLocation.ToString()));
			}
		}
	}
}

void ANXAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) == true)
	{
		BehaviorTreeComponent->StopTree();

		if (ShowAIDebug == 1)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("EndAI()")));
		}
	}
}

void ANXAIController::UpdatePatrolLocation()
{
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent))
	{
		APawn* ControlledPawn = GetPawn();
		if (IsValid(ControlledPawn))
		{
			UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			if (IsValid(NavSystem))
			{
				FNavLocation OutNavLocation; // FNavLocation ���
				FVector RandomLocation = ControlledPawn->GetActorLocation(); // �⺻�� ����

				bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(ControlledPawn->GetActorLocation(), PatrolRadius, OutNavLocation);
				if (bFoundLocation)
				{
					RandomLocation = OutNavLocation.Location;
				}

				BlackboardComponent->SetValueAsVector(EndPatrolPositionKey, RandomLocation);

				if (ShowAIDebug == 1)
				{
					UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("UpdatePatrolLocation() - ���ο� ���� ��ġ: %s"), *RandomLocation.ToString()));
				}
			}
		}
	}
}