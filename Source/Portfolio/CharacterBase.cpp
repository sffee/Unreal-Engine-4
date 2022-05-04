#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
	: m_AttackMove(false)
	, m_HitEffect(false)
	, m_Damage(false)
	, m_IsSlowTime(false)
	, m_CurSlowPower(0.f)
	, m_CurSlowTime(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HitEffectUpdate(DeltaTime);
	SlowTimeCheck();
	AttackMove();
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::SpawnProjectile(TSubclassOf<AProjectile> _Particle, const FVector& _Position, const FRotator& _Rotation, const FVector& _Velocity)
{
	FActorSpawnParameters SpawnParam = {};

	SpawnParam.OverrideLevel = GetLevel();
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(_Particle, _Position, _Rotation, SpawnParam);

	Projectile->GetProjectileMovement()->Velocity = _Velocity;

	Projectile->FinishSpawning(Projectile->GetTransform());
}

void ACharacterBase::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	HitEffect();
}

void ACharacterBase::HitEffect()
{
	m_HitEffect = true;
	m_HitEffectTimer = 0.2f;
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitEffect_Ratio"), 1.f);

	for (int i = 0; i < m_EtcMesh.Num(); i++)
		m_EtcMesh[i]->SetScalarParameterValueOnMaterials(TEXT("HitEffect_Ratio"), 1.f);
}

void ACharacterBase::HitEffectUpdate(float _DeltaTime)
{
	if (m_HitEffect == false)
		return;

	m_HitEffectTimer -= _DeltaTime;

	if (m_HitEffectTimer <= 0.f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitEffect_Ratio"), 0.f);

		for (int i = 0; i < m_EtcMesh.Num(); i++)
			m_EtcMesh[i]->SetScalarParameterValueOnMaterials(TEXT("HitEffect_Ratio"), 0.f);
	}
}

void ACharacterBase::SlowTime(float _Power, float _Time)
{
	check(_Power != 0.f);

	m_IsSlowTime = true;
	m_CurSlowTime = _Time;
	m_CurSlowPower = _Power;
	GetWorldSettings()->SetTimeDilation(_Power);
}

void ACharacterBase::SlowTimeCheck()
{
	if (m_IsSlowTime == false)
		return;

	m_CurSlowTime -= 1.f / m_CurSlowPower * GetWorld()->GetDeltaSeconds();
	if (m_CurSlowTime <= 0.f)
	{
		m_IsSlowTime = false;
		GetWorldSettings()->SetTimeDilation(1.f);
	}
}

void ACharacterBase::AttackMove()
{
	if (m_AttackMove == false)
		return;

	AddMovementInput(GetActorForwardVector(), 1.f);
}