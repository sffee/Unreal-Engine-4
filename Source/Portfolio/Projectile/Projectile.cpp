#include "Projectile.h"

#include "../Manager/LevelStreamManager.h"

AProjectile::AProjectile()
	: m_LifeTime(0.f)
	, m_LifeTimeEnable(false)
	, m_Damage(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	SetRootComponent(m_Box);
	m_ParticleComponent->SetupAttachment(m_Box);
	m_ProjectileMovement->SetUpdatedComponent(m_Box);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_LifeTime != 0.f)
		m_LifeTimeEnable = true;

	m_Box->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_LifeTimeEnable)
	{
		m_LifeTime -= DeltaTime;
		if (m_LifeTime <= 0.f)
			Destroy();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{
}

void AProjectile::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	Destroy();
}