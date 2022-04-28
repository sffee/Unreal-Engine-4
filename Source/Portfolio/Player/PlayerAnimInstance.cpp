// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "MyPlayer.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	: m_State(EPLAYER_STATE::SWORD_IDLE_L)
{
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	
}

void UPlayerAnimInstance::NativeUpdateAnimation(float _fDT)
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		m_State = Player->GetState();
		m_Speed = Player->GetCharacterMovement()->Velocity.Size();
	}
}

void UPlayerAnimInstance::AnimNotify_Idle_L()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::SWORD_IDLE_L);
	}
}

void UPlayerAnimInstance::AnimNotify_Idle_R()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::SWORD_IDLE_R);
	}
}

void UPlayerAnimInstance::AnimNotify_Moveable()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetMoveable(true);
	}
}

void UPlayerAnimInstance::AnimNotify_MoveDisable()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetMoveable(false);
	}
}

void UPlayerAnimInstance::AnimNotify_MoveStart()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetAttackMove(true);
		Player->AttackMoveSpeedSetting(m_State);
	}
}

void UPlayerAnimInstance::AnimNotify_MoveStop()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetAttackMove(false);
	}
}

void UPlayerAnimInstance::AnimNotify_Attackable()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetAttackCancleable(true);
	}
}

UFUNCTION()
void UPlayerAnimInstance::AnimNotify_CollisionOn()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->CollisionOn();
	}
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->Attack();
	}
}

void UPlayerAnimInstance::AnimNotify_RunLoop()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::SWORD_RUN, true);
	}
}

void UPlayerAnimInstance::AnimNotify_JumpLoop()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::JUMP_LOOP);
	}
}

void UPlayerAnimInstance::AnimNotify_UpperStartEnd()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		Player->Upper();
	}
}

void UPlayerAnimInstance::AnimNotify_Jump()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->Jump();
	}
}

void UPlayerAnimInstance::AnimNotify_DashAttackRunStart()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		m_State = EPLAYER_STATE::SWORD_DASHATTACK_RUN_START;
		Player->ChangeState(EPLAYER_STATE::SWORD_DASHATTACK_RUN_START);
	}
}

void UPlayerAnimInstance::AnimNotify_DashAttackRunLoop()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::SWORD_DASHATTACK_RUN_LOOP);
	}
}

void UPlayerAnimInstance::AnimNotify_DashAttackFinishEnd()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::SWORD_DASHATTACK_FINISH_END);
	}
}

void UPlayerAnimInstance::AnimNotify_JumpDownAttackLoop()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->GetCharacterMovement()->Velocity = FVector(0.f, 0.f, -2500.f);
		Player->ChangeState(EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_LOOP);
	}
}

void UPlayerAnimInstance::AnimNotify_LoopAttackStart()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetLoopAttackCheck(true);
	}
}

void UPlayerAnimInstance::AnimNotify_LoopAttackEnd()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetLoopAttackCheck(false);
	}
}

void UPlayerAnimInstance::AnimNotify_DamageEnd()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetDamage(false);
	}
}