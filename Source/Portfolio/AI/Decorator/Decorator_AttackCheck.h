#pragma once

#include "../../Etc/global.h"
#include "../../Etc/define.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Decorator_AttackCheck.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FAttackData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float NearDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float FarDistance;
};

UCLASS()
class PORTFOLIO_API UDecorator_AttackCheck : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAttackData m_AttackData;

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
