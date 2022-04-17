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

void UPlayerAnimInstance::AnimNotify_Attack()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->Attack();
	}
}