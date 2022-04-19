#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TargetComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class PORTFOLIO_API UTargetComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UTargetComponent();
};