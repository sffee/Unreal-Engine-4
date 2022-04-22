#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class PORTFOLIO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UBehaviorTree* m_BehaviorTree;
	UBlackboardData* m_Blackboard;

public:
	AEnemyAIController();

public:
	virtual void OnPossess(APawn* _Enemy) override;
	virtual void OnUnPossess() override;
};
