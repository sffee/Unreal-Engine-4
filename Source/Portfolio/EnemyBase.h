#pragma once

#include "Enemy/EnemyAnimInstance.h"

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

	UDataTable* m_MontageTable;
	TMap<EENEMY_STATE, FName> m_MontageMap;

	UEnemyAnimInstance* m_AnimInst;

	bool m_HitEffect;
	float m_HitEffectTimer;
	float m_HitEffectRatio;

protected:
	bool m_FlyDownCheck;

public:
	void SetFlyDownCheck(bool _Set)
	{
		m_FlyDownCheck = _Set;
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
	void HitEffectUpdate(float _DeltaTime);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
