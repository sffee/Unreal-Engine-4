#include "Task_Attack.h"

#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "../../Enemy/EnemyAIController.h"
#include "../../Player/MyPlayer.h"
#include "../../EnemyBase.h"

UTask_Attack::UTask_Attack()
{

}

EBTNodeResult::Type UTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
		return EBTNodeResult::Failed;

	AEnemyBase* Enemy = Cast<AEnemyBase>(Controller->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BB = Controller->GetBlackboardComponent();
	BB->SetValueAsBool(TEXT("Attacking"), true);

	EENEMY_STATE NextState = (EENEMY_STATE)BB->GetValueAsEnum(TEXT("NextState"));
	float Cooltime = Enemy->GetCooltime(NextState);

	Enemy->LookToPlayer();

	if (NextState == EENEMY_STATE::ATTACK1)
		BB->SetValueAsFloat(TEXT("Attack1Cooltime"), Cooltime);
	else if (NextState == EENEMY_STATE::ATTACK2)
		BB->SetValueAsFloat(TEXT("Attack2Cooltime"), Cooltime);
	else if (NextState == EENEMY_STATE::ATTACK3)
		BB->SetValueAsFloat(TEXT("Attack3Cooltime"), Cooltime);
	else if (NextState == EENEMY_STATE::ATTACK4)
		BB->SetValueAsFloat(TEXT("Attack4Cooltime"), Cooltime);

	Enemy->ChangeState(NextState);

	return EBTNodeResult::Succeeded;
}

void UTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	return Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}


