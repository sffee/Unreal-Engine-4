#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EnemySpawnTrigger.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSpawnEnemy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEnemyBase> SpawnEnemyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int SpawnCount;
};

USTRUCT(Atomic, BlueprintType)
struct FPhase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FSpawnEnemy> SpawnEnemy;

};

UCLASS()
class PORTFOLIO_API AEnemySpawnTrigger : public ATriggerBox
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	TArray<class AEnemySpawnner*> m_EnemySpawnners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	float m_SpawnRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	TArray<FPhase> m_Phase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	AActor* m_EndDestroyActor;

private:
	int m_PhaseCount;
	bool m_EndPhase;

private:
	bool m_StartSpawn;
	int m_AliveEnemyCount;

private:
	float m_Timer;

public:
	AEnemySpawnTrigger();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float _DeltaTime) override;

public:
	void Delegate_EnemyDie()
	{
		m_AliveEnemyCount--;
	}

	void Spawn();

private:
	UFUNCTION()
	void BeginTrigger(AActor* _TriggerActor, AActor* _OtherActor);
};