#pragma once

#include <Components/WidgetComponent.h>
#include "../UI/LockOnWidget.h"

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TargetComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class PORTFOLIO_API UTargetComponent : public USphereComponent
{
	GENERATED_BODY()

private:
	UWidgetComponent* m_LockOnWidgetComponent;

public:
	void LockOn();
	void LockOff();

public:
	UTargetComponent();
};