#include "EnemySpawnner.h"

#include "../../EnemyBase.h"

// Sets default values
AEnemySpawnner::AEnemySpawnner()
{
}

void AEnemySpawnner::BeginPlay()
{
	Super::BeginPlay();

	
}

AEnemyBase* AEnemySpawnner::SpawnStart(TSubclassOf<AEnemyBase>& _SpawnEnemy, float _Range)
{
	FActorSpawnParameters SpawnParam = {};
	SpawnParam.OverrideLevel = GetLevel();
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	float Dist = FMath::RandRange(0.f, _Range);
	float Angle = FMath::RandRange(0.f, 2.f * PI);
	FVector RandomPos = GetActorLocation();
	RandomPos += FVector(FMath::Sin(Angle) * Dist, FMath::Cos(Angle) * Dist, 100.f);

	AEnemyBase* Spawnner = GetWorld()->SpawnActor<AEnemyBase>(_SpawnEnemy, FTransform(GetActorRotation(), RandomPos), SpawnParam);

	Spawnner->FinishSpawning(Spawnner->GetTransform());

	DrawDebugSphere(GetWorld(), GetActorLocation(), _Range, 40, FColor::Red, false, 1.f);

	return Spawnner;
}