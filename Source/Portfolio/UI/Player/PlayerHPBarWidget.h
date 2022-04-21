#pragma once

#include "../../Etc/global.h"

#include <Components/ProgressBar.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHPBarWidget.generated.h"

UCLASS()
class PORTFOLIO_API UPlayerHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar* m_HPBar;

public:
	void SetHP(float _Ratio)
	{
		m_HPBar->SetPercent(_Ratio);
	}

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
