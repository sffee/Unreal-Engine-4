#pragma once

#include "Etc/global.h"

#include "Manager/LevelStreamManager.h"
#include "Projectile/Projectile.h"

#include "UI/DamageTextActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class PORTFOLIO_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

private:
	TSubclassOf<ADamageTextActor> m_DamageTextActor;

protected:
	UDataTable* m_MontageTable;

	UDataTable* m_AttackMoveTable;
	UDataTable* m_AttackInfoTable;

protected:
	TArray<USkeletalMeshComponent*> m_EtcMesh;

private:
	bool m_AttackMove;

protected:
	bool m_HitEffect;
	float m_HitEffectTimer;

	bool m_Damage;

	bool m_IsSlowTime;
	float m_CurSlowPower;
	float m_CurSlowTime;

public:
	bool IsDamage()
	{
		return m_Damage;
	}

	void SetDamage(bool _Set)
	{
		m_Damage = _Set;
	}

	void SetAttackMove(bool _Set)
	{
		m_AttackMove = _Set;
	}

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo, bool _Player = false);
	void SpawnProjectile(TSubclassOf<AProjectile> _Particle, const FVector& _Position, const FRotator& _Rotation, const FVector& _Velocity);

protected:
	void HitEffect();

public:
	void SlowTime(float _Power, float _Time);

private:
	void SlowTimeCheck();
	void AttackMove();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HitEffectUpdate(float _DeltaTime);
};