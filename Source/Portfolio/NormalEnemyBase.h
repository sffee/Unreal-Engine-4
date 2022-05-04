#pragma once

#include <Components/WidgetComponent.h>

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "NormalEnemyBase.generated.h"

UCLASS()
class PORTFOLIO_API ANormalEnemyBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	ANormalEnemyBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_WidgetComponent;

private:
	void KnockBackFlyLandCheck();

protected:
	void SetHP(float _Ratio);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};