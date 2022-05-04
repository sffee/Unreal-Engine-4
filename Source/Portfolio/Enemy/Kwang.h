#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "Kwang.generated.h"

UCLASS()
class PORTFOLIO_API AKwang : public AEnemyBase
{
	GENERATED_BODY()
	
private:
	float m_WaitTime;

public:
	AKwang();

private:
	void StateUpdate();
	void Attack4Update();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};