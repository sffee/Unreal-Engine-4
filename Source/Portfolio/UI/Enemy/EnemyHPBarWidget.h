#pragma once

#include "../../Etc/global.h"

#include <Components/ProgressBar.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

UCLASS()
class PORTFOLIO_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UProgressBar* m_HPBar;

public:
	void SetHP(float _Ratio)
	{
		m_HPBar->SetPercent(_Ratio);
	}

	void SetVisible(bool _Set)
	{
		if (_Set == true)
			this->Visibility = ESlateVisibility::Visible;
		else
			this->Visibility = ESlateVisibility::Hidden;
	}

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};