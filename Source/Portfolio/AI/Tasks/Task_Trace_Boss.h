#pragma once

#include "../../Etc/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Trace_Boss.generated.h"

UCLASS()
class PORTFOLIO_API UTask_Trace_Boss : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_Trace_Boss();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
