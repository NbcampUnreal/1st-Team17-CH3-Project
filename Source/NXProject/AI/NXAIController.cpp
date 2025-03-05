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
	//BrainComponent는 AIController 클래스에 정의된 속성. UBrainComponent <- UBehaviorTreeComponent
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
		GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &ANXAIController::UpdatePatrolLocation, 5.0f, true); // 5초마다 순찰 위치 업데이트
	}
}

void ANXAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(InPawn))
	{
		BeginAI(InPawn); // 스폰된 후에도 AI 초기화 실행
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

			// 네비게이션 시스템 가져오기
			UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
			FVector RandomLocation = InPawn->GetActorLocation(); // 기본값 설정

			if (IsValid(NavSystem))
			{
				// 순찰 종료 위치 찾기
				FNavLocation OutNavLocation;
				bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(InPawn->GetActorLocation(), PatrolRadius, OutNavLocation);
				if (bFoundLocation)
				{
					RandomLocation = OutNavLocation.Location;
				}
			}

			BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
			BlackboardComponent->SetValueAsVector(EndPatrolPositionKey, RandomLocation); // 수정된 부분

			if (ShowAIDebug == 1)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI() - 순찰 시작 위치: %s"), *InPawn->GetActorLocation().ToString()));
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("BeginAI() - 순찰 종료 위치: %s"), *RandomLocation.ToString()));
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
				FNavLocation OutNavLocation; // FNavLocation 사용
				FVector RandomLocation = ControlledPawn->GetActorLocation(); // 기본값 설정

				bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(ControlledPawn->GetActorLocation(), PatrolRadius, OutNavLocation);
				if (bFoundLocation)
				{
					RandomLocation = OutNavLocation.Location;
				}

				BlackboardComponent->SetValueAsVector(EndPatrolPositionKey, RandomLocation);

				if (ShowAIDebug == 1)
				{
					UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("UpdatePatrolLocation() - 새로운 순찰 위치: %s"), *RandomLocation.ToString()));
				}
			}
		}
	}
}