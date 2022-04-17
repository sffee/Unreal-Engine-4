#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "Murdock.generated.h"

UCLASS()
class PORTFOLIO_API AMurdock : public AEnemyBase
{
	GENERATED_BODY()
	
public:
	AMurdock();

public:
	virtual void Damage(AActor* _Actor) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};