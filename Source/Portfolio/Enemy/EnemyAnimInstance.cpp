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