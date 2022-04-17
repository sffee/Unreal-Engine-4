#pragma once

#include "Enemy/EnemyAnimInstance.h"

#include "Etc/define.h"

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

	UDataTable* m_MontageTable;
	TMap<EENEMY_STATE, FName> m_MontageMap;

	UEnemyAnimInstance* m_AnimInst;

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
	virtual void Damage(AActor* _Actor) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
