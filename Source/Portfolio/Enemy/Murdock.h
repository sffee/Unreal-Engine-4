#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "Murdock.generated.h"

UCLASS()
class PORTFOLIO_API AMurdock : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AMurdock();

private:
	void KnockBackFlyLandCheck();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};