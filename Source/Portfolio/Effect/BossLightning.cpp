#include "BossLightning.h"

#include "../Manager/LevelStreamManager.h"
#include "../Player/MyPlayer.h"

ABossLightning::ABossLightning()
	: m_LifeTime(2.f)
	, m_Attack(false)
	, m_AttackTime(0.1f)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	SetRootComponent(m_Particle);
}

void ABossLightning::BeginPlay()
{
	Super::BeginPlay();
	
	m_Particle->SetTemplate(Cast<UParticleSystem>(ULevelStreamManager::GetInst(GetWorld())->FindAsset("/Game/ParagonKwang/FX/Particles/Abilities/LightStrike/FX/P_Kwang_LightStrike_Burst.P_Kwang_LightStrike_Burst")));
}

void ABossLightning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Attack == false)
	{
		m_AttackTime -= DeltaTime;
		if (m_AttackTime <= 0.f)
		{
			m_Attack = true;

			FVector vPos = GetActorLocation();

			TArray<FHitResult> arrHit;
			FCollisionQueryParams param(NAME_None, false, this);

			GetWorld()->SweepMultiByChannel(arrHit, vPos, vPos, FQuat::Identity
				, ECC_GameTraceChannel8
				, FCollisionShape::MakeSphere(180.f), param);

			if (arrHit.Num())
			{
				FAttackInfo AttackInfo;
				AttackInfo.Damage = 30.f;
				AttackInfo.KnockBackPowerXY = 750.f;

				Cast<AMyPlayer>(arrHit[0].Actor)->Damage(this, &AttackInfo);
			}

			DrawDebugSphere(GetWorld(), vPos, 180.f, 20, FColor::Red, false, 0.5f);
		}
	}

	m_LifeTime -= DeltaTime;
	if (m_LifeTime <= 0.f)
		Destroy();
}