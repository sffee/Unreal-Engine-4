#pragma once

#include "Enemy/EnemyAnimInstance.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>

#include "Etc/define.h"
#include "Etc/MyStruct.h"

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

DECLARE_DELEGATE(FSpawnEnemyDieDelegate)

UCLASS()
class PORTFOLIO_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	FSpawnEnemyDieDelegate Die_Delegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	FEnemyInfo m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* m_Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	class UTargetComponent* m_TargetComponent;

	float m_CooltimeWaitDistance;

	UDataTable* m_MontageTable;
	TMap<EENEMY_STATE, FName> m_MontageMap;

	bool m_Dissolve;
	float m_DissolveProgress;

	bool m_Attack;
	bool m_Fly;

	float m_AttackCooltime[6];
	float m_RemainAttackCooltime[6];

protected:
	bool m_LoopAttackCheck;
	TArray<AActor*> m_AttackHitActors;

	bool m_FlyDownCheck;

public:
	void SetFlyDownCheck(bool _Set)
	{
		m_FlyDownCheck = _Set;
	}

	UBlackboardData* GetBlackboard()
	{
		return m_Blackboard;
	}

	UBehaviorTree* GetBehaviorTree()
	{
		return m_BehaviorTree;
	}

	void SetAttack(bool _Set)
	{
		m_Attack = _Set;
	}

	bool IsAttack()
	{
		return m_Attack;
	}

	float GetCooltimeWaitDistance()
	{
		return m_CooltimeWaitDistance;
	}

public:
	AEnemyBase();

public:
	EENEMY_STATE GetState()
	{
		return m_State;
	}

protected:
	void PlayMontage(EENEMY_STATE _State);

public:
	void ChangeState(EENEMY_STATE _NextState, bool _Ignore = false);

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo) override;
	void Dissolve();

public:
	virtual void Attack();

	virtual bool AttackCheck()
	{
		return false;
	};

	virtual void Fire() {};

protected:
	void LookToPlayer();
	FRotator GetRotationToPlayer();

private:
	void DissolveUpdate(float _DeltaTime);
	bool HitProcess(const FHitResult& _HitResult, const FAttackInfo* _AttackInfo);
	void CooltimeUpdate(float _DeltaTime);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
