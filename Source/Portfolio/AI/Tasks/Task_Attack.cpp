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

	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (Player == nullptr)
		return EBTNodeResult::Failed;

	AEnemyBase* Enemy = Cast<AEnemyBase>(Controller->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	EENEMY_STATE State = Enemy->GetState();
	if (State == EENEMY_STATE::SPAWN || State == EENEMY_STATE::DEATH)
		return EBTNodeResult::Failed;

	if (Enemy->Attack())
	{
		Controller->StopMovement();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	return Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}


