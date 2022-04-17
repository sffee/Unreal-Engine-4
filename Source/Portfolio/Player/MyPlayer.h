// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerAnimInstance.h"

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "MyPlayer.generated.h"

UCLASS()
class PORTFOLIO_API AMyPlayer : public ACharacterBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_Arm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_State;

	UDataTable* m_MontageTable;
	TMap<EPLAYER_STATE, FName> m_MontageMap;

	UDataTable* m_AttackMoveTable;
	UDataTable* m_AttackInfoTable;

	UPlayerAnimInstance* m_AnimInst;

	bool m_Attacking;
	bool m_AttackMove;
	bool m_AttackCancleable;
	bool m_Moveable;
	
	bool m_PressMoveSide;
	bool m_PressMoveFront;
	int m_ComboBCount;

	bool m_IsSlowTime;
	float m_CurSlowPower;
	float m_CurSlowTime;

public:
	AMyPlayer();

	EPLAYER_STATE GetState()
	{
		return m_State;
	}

	void SetAttackMove(bool _Set)
	{
		m_AttackMove = _Set;
	}

	void SetAttackCancleable(bool _Set)
	{
		m_AttackCancleable = _Set;
	}

	void SetMoveable(bool _Set)
	{
		m_Moveable = _Set;
	}

private:
	void PlayMontage(EPLAYER_STATE _State);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void TableSetting();

public:
	void ChangeState(EPLAYER_STATE _NextState, bool _Ignore = false);
	void AttackMoveSpeedSetting(EPLAYER_STATE _State);
	void CheckRunAnimation();

public:
	void Attack();
	bool HitProcess(const FHitResult& _HitResult, const FAttackInfo* _AttackInfo);
	void SlowTime(float _Power, float _Time);
	void SlowTimeCheck();

private:
	void AttackMove();

private:
	void MoveFront(float _Scale);
	void MoveSide(float _Scale);
	void RotationZ(float _Scale);
	void RotationY(float _Scale);

private:
	void JumpAction();
	void AttackAction();
	void AttackRAction();
};
