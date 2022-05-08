#pragma once

#include "../../Etc/global.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Service_CooltimeUpdater.generated.h"

UCLASS()
class PORTFOLIO_API UService_CooltimeUpdater : public UBTService
{
	GENERATED_BODY()
	
public:
	UService_CooltimeUpdater();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};