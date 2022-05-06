#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossLightning.generated.h"

UCLASS()
class PORTFOLIO_API ABossLightning : public AActor
{
	GENERATED_BODY()
	
private:
	UParticleSystemComponent* m_Particle;
	float m_LifeTime;
	
	bool m_Attack;
	float m_AttackTime;

public:	
	ABossLightning();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
