#pragma once

#include "CoreMinimal.h"
#include "InteractionBase.h"
#include "Portal.generated.h"

UCLASS()
class PORTFOLIO_API APortal : public AInteractionBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* m_ParticleComponent;
	
public:
	APortal();

public:
	virtual void Interact() override;
};
