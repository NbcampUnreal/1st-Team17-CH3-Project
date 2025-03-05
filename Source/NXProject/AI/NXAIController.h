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

	friend class ANXNonPlayerCharacter;		// ���� ó��

public:

	ANXAIController();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	//--------BB_NPC�� BT_NPC �ּ��� AIController Ŭ������ ����---------//

	void BeginAI(APawn* InPawn);
	void EndAI();

	void UpdatePatrolLocation(); // ���� ��ġ ������Ʈ �Լ�

public:

	static const float PatrolRadius;
	static int32 ShowAIDebug;
	virtual void OnPossess(APawn* InPawn) override;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	//-----------AIController Ŭ�������� ������ �� ����---------------//

	FTimerHandle PatrolTimerHandle; // ����: PatrolTimerHandle �߰�

public:

	static const FName StartPatrolPositionKey;
	static const FName EndPatrolPositionKey;
	static const FName TargetCharacterKey;

};

