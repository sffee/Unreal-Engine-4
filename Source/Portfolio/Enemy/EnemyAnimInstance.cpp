#include "EnemyAnimInstance.h"
#include "../EnemyBase.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{

}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
}

void UEnemyAnimInstance::NativeBeginPlay()
{
}

void UEnemyAnimInstance::NativeUpdateAnimation(float _fDT)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		m_State = Enemy->GetState();
	}
}

void UEnemyAnimInstance::AnimNotify_Idle()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->ChangeState(EENEMY_STATE::IDLE);
	}
}

void UEnemyAnimInstance::AnimNotify_Attack()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->Attack();
	}
}

void UEnemyAnimInstance::AnimNotify_Fire()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->Fire();
	}
}

void UEnemyAnimInstance::AnimNotify_FlyDownCheck()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->SetFlyDownCheck(true);
	}
}

void UEnemyAnimInstance::AnimNotify_Destroy()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->Destroy();
	}
}

void UEnemyAnimInstance::AnimNotify_Dissolve()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->Dissolve();
	}
}

void UEnemyAnimInstance::AnimNotify_DamageAirEnd()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->GetCharacterMovement()->GravityScale = 1.f;
		Enemy->ChangeState(EENEMY_STATE::DAMAGE_KNOCKBACK_FLY);
	}
}

void UEnemyAnimInstance::AnimNotify_DecreaseVelocity()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->GetCharacterMovement()->Velocity /= 1.6f;
	}
}

void UEnemyAnimInstance::AnimNotify_MoveStart()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->SetAttackMove(true);
		Enemy->AttackMoveSpeedSetting(m_State);
	}
}

void UEnemyAnimInstance::AnimNotify_MoveStop()
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Enemy != nullptr)
	{
		Enemy->SetAttackMove(false);
	}
}