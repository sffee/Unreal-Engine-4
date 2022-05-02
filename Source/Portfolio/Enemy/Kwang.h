#pragma once

#include "BossAnimInstance.h"

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "Kwang.generated.h"

UCLASS()
class PORTFOLIO_API AKwang : public AEnemyBase
{
	GENERATED_BODY()
	
private:
	UBossAnimInstance* m_AnimInst;

public:
	AKwang();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo) override;
	virtual bool AttackCheck() override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};