// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance()
	: m_State(EPLAYER_STATE::IDLE)
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
	}
}

void UPlayerAnimInstance::AnimNotify_ActionEnd()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::IDLE);
	}
}

void UPlayerAnimInstance::AnimNotify_Run_End()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->ChangeState(EPLAYER_STATE::IDLE);
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

void UPlayerAnimInstance::AnimNotify_Cancleable()
{
	AMyPlayer* Player = Cast<AMyPlayer>(TryGetPawnOwner());
	if (Player != nullptr)
	{
		Player->SetAttackCancleable(true);
	}
}