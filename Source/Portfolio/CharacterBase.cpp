#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
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