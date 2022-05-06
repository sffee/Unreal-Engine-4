#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarningMark.generated.h"

UCLASS()
class PORTFOLIO_API AWarningMark : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* m_Decal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_LifeTime;

	float m_CheckLifeTime;
	bool m_CreateEffect;

public:	
	AWarningMark();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
