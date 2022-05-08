#pragma once

#include "CoreMinimal.h"
#include "../NormalEnemyBase.h"
#include "Minions.generated.h"

UCLASS()
class PORTFOLIO_API AMinions : public ANormalEnemyBase
{
	GENERATED_BODY()
	
public:
	AMinions();

private:
	void SpawnUpdate();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo, bool _Player = false) override;
	virtual bool AttackCheck() override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};