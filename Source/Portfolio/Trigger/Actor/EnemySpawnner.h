#pragma once

#include "../../Etc/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnner.generated.h"

UCLASS()
class PORTFOLIO_API AEnemySpawnner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnner();

public:
	class AEnemyBase* SpawnStart(TSubclassOf<class AEnemyBase>& _SpawnEnemy, float _Range);

protected:
	virtual void BeginPlay() override;
};