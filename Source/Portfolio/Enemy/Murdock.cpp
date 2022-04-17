#include "Murdock.h"
#include "../Player/MyPlayer.h"

AMurdock::AMurdock()
{
	
}

void AMurdock::BeginPlay()
{
	Super::BeginPlay();
}

void AMurdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KnockBackFlyLandCheck();
}

void AMurdock::KnockBackFlyLandCheck()
{
	if (m_State != EENEMY_STATE::DAMAGE_KNOCKBACK_FLY || m_FlyDownCheck == false)
		return;

	if (GetMovementComponent()->IsMovingOnGround())
		ChangeState(EENEMY_STATE::DOWN);
}

void AMurdock::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);

	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _Actor->GetActorLocation());;
	Rotator.Roll = 0.f;
	Rotator.Pitch = 0.f;
	SetActorRotation(Rotator);

	FVector HitVector = _Actor->GetActorForwardVector();
	HitVector.X *= _AttackInfo->KnockBackPowerXY;
	HitVector.Y *= _AttackInfo->KnockBackPowerXY;
	HitVector.Z = _AttackInfo->KnockBackPowerZ;
	LaunchCharacter(HitVector, false, false);

	if (0.f < _AttackInfo->KnockBackPowerZ)
		ChangeState(EENEMY_STATE::DAMAGE_KNOCKBACK_FLY);
	else
		ChangeState(EENEMY_STATE::DAMAGE_KNOCKBACK_GROUND);
}