#pragma once

#include <Components/WidgetComponent.h>

#include "../Etc/global.h"

#include "DamageTextWidget.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

UCLASS()
class PORTFOLIO_API ADamageTextActor : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* m_DamageTextWidget;
	
	float m_LifeTime;
	bool m_DamageTextChanged;
	float m_Damage;
	bool m_Player;

public:
	void SetDamageText(int _Damage, bool _Player)
	{
		m_Damage = _Damage;
		m_Player = _Player;
	}

public:	
	ADamageTextActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};