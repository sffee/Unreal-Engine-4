#include "EnemyAIController.h"
#include "../EnemyBase.h"

AEnemyAIController::AEnemyAIController()
	: m_BehaviorTree(nullptr)
	, m_Blackboard(nullptr)
{

}

void AEnemyAIController::OnPossess(APawn* _Enemy)
{
	Super::OnPossess(_Enemy);

	AEnemyBase* Enemy = Cast<AEnemyBase>(_Enemy);
	if (Enemy != nullptr)
	{
		m_BehaviorTree = Enemy->GetBehaviorTree();
		m_Blackboard = Enemy->GetBlackboard();

		UseBlackboard(m_Blackboard, Blackboard);
		RunBehaviorTree(m_BehaviorTree);
	}
}

void AEnemyAIController::OnUnPossess()
{

}