// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Etc/global.h"

#include "PlayerAnimInstance.h"

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "MyPlayer.generated.h"

UENUM()
enum class EMONTAGE : uint8
{
	RUN,
	RUN_END,
	COMBO_A_1,
	COMBO_A_2,
	COMBO_A_3,
	COMBO_A_4,
	COMBO_A_5,
	COMBO_A_6
};

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
	
	UPlayerAnimInstance* m_AnimInst;

	TArray<UAnimMontage*> m_arrMontage;

	bool m_AttackMove;
	bool m_AttackCancleable;
	bool m_Moveable;

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
	void AddAnimMontage(const FString _FileName);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void ChangeState(EPLAYER_STATE _NextState);

private:
	void AttackMove();
	void AttackMoveSpeedSetting(EPLAYER_STATE _State);

private:
	void MoveFront(float _Scale);
	void MoveSide(float _Scale);
	void RotationZ(float _Scale);
	void RotationY(float _Scale);

private:
	void JumpAction();
	void AttackAction();
};
