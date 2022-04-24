#include "EnemySpawnTrigger.h"

#include "Actor/EnemySpawnner.h"
#include "../EnemyBase.h"

AEnemySpawnTrigger::AEnemySpawnTrigger()
	: m_StartSpawn(false)
	, m_AliveEnemyCount(0)
	, m_PhaseCount(0)
	, m_EndPhase(false)
	, m_Timer(0.3f)
{
	PrimaryActorTick.bCanEverTick = true;

	GetCollisionComponent()->SetCollisionProfileName(TEXT("SpawnTrigger"));

	OnActorBeginOverlap.AddDynamic(this, &AEnemySpawnTrigger::BeginTrigger);
}

void AEnemySpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawnTrigger::Tick(float _DeltaTime)
{
	if (m_StartSpawn == false)
	{
		m_Timer += _DeltaTime;
		if (0.3f <= m_Timer)
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, false, 0.31f);
			m_Timer = 0.f;
		}

		return;
	}

	if (m_AliveEnemyCount == 0)
	{
		if (m_EndPhase)
		{
			if (m_EndDestroyActor != nullptr)
				m_EndDestroyActor->Destroy();

			Destroy();
		}
		else
			Spawn();
	}
}

void AEnemySpawnTrigger::BeginTrigger(AActor* _TriggerActor, AActor* _OtherActor)
{
	if (m_StartSpawn)
		return;

	Spawn();
}

void AEnemySpawnTrigger::Spawn()
{
	TArray<int> SpawnPoint;

	for (int i = 0; i < m_Phase[m_PhaseCount].SpawnEnemy.Num(); i++)
	{
		FSpawnEnemy& SpawnEnemy = m_Phase[m_PhaseCount].SpawnEnemy[i];

		for (int j = 0; j < SpawnEnemy.SpawnCount; j++)
		{
			if (SpawnPoint.Num() == 0)
			{
				for (int k = 0; k < m_EnemySpawnners.Num(); k++)
					SpawnPoint.Add(k);
			}

			int RandomPoint = SpawnPoint[FMath::RandRange(0, SpawnPoint.Num() - 1)];
			SpawnPoint.RemoveAt(SpawnPoint.Find(RandomPoint));

			AEnemyBase* Enemy = m_EnemySpawnners[RandomPoint]->SpawnStart(SpawnEnemy.SpawnEnemyType, m_SpawnRange);
			m_AliveEnemyCount++;

			Enemy->Die_Delegate.BindUObject(this, &AEnemySpawnTrigger::Delegate_EnemyDie);
		}
	}

	if (m_Phase.Num() - 1 == m_PhaseCount)
		m_EndPhase = true;
	else
		m_PhaseCount++;

	m_StartSpawn = true;
}

