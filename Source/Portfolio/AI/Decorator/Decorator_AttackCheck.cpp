#include "Decorator_AttackCheck.h"

#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "../../Enemy/EnemyAIController.h"
#include "../../Player/MyPlayer.h"
#include "../../EnemyBase.h"

bool UDecorator_AttackCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
		return false;

	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (Player == nullptr)
		return false;

	AEnemyBase* Enemy = Cast<AEnemyBase>(Controller->GetPawn());
	if (Enemy == nullptr)
		return false;

	float Distance = FVector::Distance(Player->GetActorLocation(), Enemy->GetActorLocation());
	if (m_AttackData.NearDistance <= Distance && Distance <= m_AttackData.FarDistance)
	{
		UBlackboardComponent* BB = Controller->GetBlackboardComponent();
		BB->SetValueAsEnum(TEXT("NextState"), (uint8)m_AttackData.State);

		return true;
	}

	return false;
}