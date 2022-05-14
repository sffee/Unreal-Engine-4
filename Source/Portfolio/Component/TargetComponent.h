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
	bool m_Death;

public:
	void Death()
	{
		m_Death = true;
	}

	bool IsDeath()
	{
		return m_Death;
	}

public:
	UTargetComponent();
	~UTargetComponent();
};