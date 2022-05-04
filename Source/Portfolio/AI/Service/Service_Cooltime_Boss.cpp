#include "Service_Cooltime_Boss.h"

#include <BehaviorTree/BlackboardComponent.h>
#include "../../Enemy/EnemyAIController.h"

UService_Cooltime_Boss::UService_Cooltime_Boss()
{
	Interval = 0.001f;

	RandomDeviation = 0.f;
}

void UService_Cooltime_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
		return;

	for (int i = 0; i < 4; i++)
	{
		FString Str = "Attack" + FString::FromInt(i + 1) + "Cooltime";
		float Cooltime = Controller->GetBlackboardComponent()->GetValueAsFloat(*Str);
		Cooltime = FMath::Max(Cooltime - DeltaSeconds, 0.f);

		Controller->GetBlackboardComponent()->SetValueAsFloat(*Str, Cooltime);
	}
}