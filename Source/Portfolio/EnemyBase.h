#pragma once

#include "Enemy/EnemyAnimInstance.h"

#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>

#include "Etc/define.h"
#include "Etc/MyStruct.h"

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "EnemyBase.generated.h"

UCLASS()
class PORTFOLIO_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	FEnemyInfo m_Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* m_Blackboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_BehaviorTree;

	class UTargetComponent* m_TargetComponent;

	UDataTable* m_MontageTable;
	TMap<EENEMY_STATE, FName> m_MontageMap;

	UEnemyAnimInstance* m_AnimInst;

	bool m_HitEffect;
	float m_HitEffectTimer;

	bool m_Dissolve;
	float m_DissolveProgress;

	bool m_Attack;
	bool m_Damage;

protected:
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

	bool IsDamage()
	{
		return m_Damage;
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
	virtual bool Attack()
	{
		return false;
	};

	virtual void Fire() {};

protected:
	void LookToPlayer();

private:
	void HitEffectUpdate(float _DeltaTime);
	void DissolveUpdate(float _DeltaTime);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
