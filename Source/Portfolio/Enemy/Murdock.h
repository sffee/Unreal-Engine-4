#pragma once

#include "../Projectile/Projectile.h"

#include "CoreMinimal.h"
#include "../NormalEnemyBase.h"
#include "Murdock.generated.h"

UCLASS()
class PORTFOLIO_API AMurdock : public ANormalEnemyBase
{
	GENERATED_BODY()

private:
	TSubclassOf<AProjectile> m_AttackProjectile;

public:
	AMurdock();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo, bool _Player = false) override;
	virtual bool AttackCheck() override;
	virtual void Fire() override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};