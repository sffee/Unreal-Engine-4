#include "TargetComponent.h"

#include "../EnemyBase.h"

UTargetComponent::UTargetComponent()
	: m_Death(false)
{
	SetCollisionProfileName("LockOn");
}

UTargetComponent::~UTargetComponent()
{
}