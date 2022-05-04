#pragma once

#include "../../Etc/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_Cooltime_Boss.generated.h"

UCLASS()
class PORTFOLIO_API UService_Cooltime_Boss : public UBTService
{
	GENERATED_BODY()
	
public:
	UService_Cooltime_Boss();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};