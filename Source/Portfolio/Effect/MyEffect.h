#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffect.generated.h"

UCLASS()
class PORTFOLIO_API AMyEffect : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_Particle;

	UObject* m_Object;

public:
	void SetObject(UObject* _Object, float _Scale);

public:	
	AMyEffect();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void OnFinish(UParticleSystemComponent* _Particle);
};