#include "Task_Trace.h"

#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "../../Enemy/EnemyAIController.h"
#include "../../Player/MyPlayer.h"
#include "../../EnemyBase.h"

UTask_Trace::UTask_Trace()
{

}

EBTNodeResult::Type UTask_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Enemy->IsAttack())
		return EBTNodeResult::Failed;

	EENEMY_STATE State = Enemy->GetState();
	if (State == EENEMY_STATE::SPAWN || State == EENEMY_STATE::DEATH)
		return EBTNodeResult::Failed;

	Enemy->ChangeState(EENEMY_STATE::RUN);

	UAIBlueprintHelperLibrary::SimpleMoveToActor(Controller, Player);

	return EBTNodeResult::Succeeded;
}

void UTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	return Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_Trace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
