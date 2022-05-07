#include "Task_Trace_Boss.h"

#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "../../Enemy/EnemyAIController.h"
#include "../../Player/MyPlayer.h"
#include "../../Enemy/Kwang.h"

UTask_Trace_Boss::UTask_Trace_Boss()
{

}

EBTNodeResult::Type UTask_Trace_Boss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
		return EBTNodeResult::Failed;

	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (Player == nullptr)
		return EBTNodeResult::Failed;

	AKwang* Boss = Cast<AKwang>(Controller->GetPawn());
	if (Boss == nullptr)
		return EBTNodeResult::Failed;

	EENEMY_STATE State = Boss->GetState();
	if (State == EENEMY_STATE::DEATH)
		return EBTNodeResult::Failed;

	float Distance = FVector::Distance(Boss->GetActorLocation(), Player->GetActorLocation());
	if (Boss->GetCooltimeWaitDistance() < Distance)
	{
		FVector PlayerPos = Player->GetActorLocation();
		PlayerPos.Z = Controller->GetPawn()->GetActorLocation().Z;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, PlayerPos);
	}
	else
	{
		Controller->StopMovement();
		Boss->ChangeState(EENEMY_STATE::IDLE);
	}

	return EBTNodeResult::Succeeded;
}

void UTask_Trace_Boss::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	return Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

EBTNodeResult::Type UTask_Trace_Boss::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}
