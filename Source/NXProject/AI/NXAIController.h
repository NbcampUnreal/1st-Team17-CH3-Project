#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NXAIController.generated.h"

/**
 *
 */
UCLASS()
class NXPROJECT_API ANXAIController : public AAIController
{
	GENERATED_BODY()

	friend class ANXNonPlayerCharacter;		// 죽음 처리

public:

	ANXAIController();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	//--------BB_NPC와 BT_NPC 애셋을 AIController 클래스에 연결---------//

	void BeginAI(APawn* InPawn);
	void EndAI();

	void UpdatePatrolLocation(); // 순찰 위치 업데이트 함수

public:

	static const float PatrolRadius;
	static int32 ShowAIDebug;
	virtual void OnPossess(APawn* InPawn) override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	//-----------AIController 클래스에서 블랙보드 값 수정---------------//

	FTimerHandle PatrolTimerHandle; // 수정: PatrolTimerHandle 추가

public:

	static const FName StartPatrolPositionKey;
	static const FName EndPatrolPositionKey;
	static const FName TargetCharacterKey;

};

