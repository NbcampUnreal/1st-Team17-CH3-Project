#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_DetectPlayerCharacter.generated.h"

UCLASS()
class NXPROJECT_API UBTService_DetectPlayerCharacter : public UBTService
{
	GENERATED_BODY()
	
//-------------------------주위 캐릭터 감지-------------------------//
public:
	UBTService_DetectPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

//------------------------------------------------------------------//
};
