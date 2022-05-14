// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerAnimInstance.h"

#include "../Interaction/InteractionBase.h"

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "MyPlayer.generated.h"

UCLASS()
class PORTFOLIO_API AMyPlayer : public ACharacterBase
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_Cam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ULockOnArmComponent* m_LockOnArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	FPlayerInfo m_Info;

	USkeletalMeshComponent* m_WingMesh;
	USkeletalMeshComponent* m_SwordMesh;

	TMap<EPLAYER_STATE, FName> m_MontageMap;

	UPlayerAnimInstance* m_AnimInst;

	bool m_Attacking;
	bool m_AttackCancleable;
	bool m_Moveable;
	
	bool m_PressWKey;
	bool m_PressSKey;
	bool m_PressAKey;
	bool m_PressDKey;
	int m_ComboBCount;
	bool m_PressSpecialAttackKey;

	bool m_PressLockOn;
	float m_PressLockOnTime;

	bool m_Jump;
	bool m_JumpSecond;
	bool m_JumpAttack;
	bool m_JumpAttackB;

	float m_DashAttackTimer;

	bool m_LoopAttackCheck;
	TArray<AActor*> m_AttackHitActors;

	AInteractionBase* m_Interaction;

public:
	AMyPlayer();

	void SetPlayerInfo(FPlayerInfo& _Info)
	{
		m_Info = _Info;
	}

	EPLAYER_STATE GetState()
	{
		return m_State;
	}

	void SetAttackCancleable(bool _Set)
	{
		m_AttackCancleable = _Set;
	}

	void SetMoveable(bool _Set)
	{
		m_Moveable = _Set;
	}

	void CollisionOn()
	{
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}

	void SetLoopAttackCheck(bool _Set)
	{
		if (_Set)
			m_AttackHitActors.Empty();

		m_LoopAttackCheck = _Set;
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
	void Upper();
	bool HitProcess(const FHitResult& _HitResult, const FAttackInfo* _AttackInfo);

private:
	void LockOnCameraUpdate(float _DeltaTime);
	void HUDUpdate();
	void JumpUpdate();
	void DashAttackUpdate(float _DeltaTime);
	void LoopAttackUpdate();

private:
	void LookToLockOnTarget();

private:
	void MoveFront(float _Scale);
	void MoveSide(float _Scale);
	void Turn(float _Scale);
	void LookUp(float _Scale);

private:
	void JumpAction();
	void AttackAction();
	void AttackRAction();
	void EvadeAction();
	void LockOnDownAction();
	void LockOnUpAction();
	void SpecialAttackDownAction();
	void SpecialAttackUpAction();
	void UsePotionAction();
	void InteractAction();

private:
	bool IsPressMoveKey();
	FRotator GetPressKeyRotation();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo, bool _Player = false) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
};