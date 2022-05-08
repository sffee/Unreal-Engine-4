#pragma once

#include "../Decal/WarningMark.h"

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "Kwang.generated.h"

UCLASS()
class PORTFOLIO_API AKwang : public AEnemyBase
{
	GENERATED_BODY()
	
private:
	TSubclassOf<AActor> m_WarningMark;
	
	float m_WaitTime;

	bool m_Lightning;
	float m_LightningTimer;

public:
	void SetLightning(bool _Set)
	{
		m_Lightning = _Set;
		m_LightningTimer = 0.f;
	}

public:
	AKwang();

public:
	virtual void ChangeState(EENEMY_STATE _NextState, bool _Ignore = false) override;

private:
	void StateUpdate();
	void Attack4Update();
	void LightningUpdate(float _DeltaTime);

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo, bool _Player = false) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};