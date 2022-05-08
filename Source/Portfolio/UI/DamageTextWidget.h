#pragma once

#include "../Etc/global.h"

#include <Components/TextBlock.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.generated.h"

UCLASS()
class PORTFOLIO_API UDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

	UTextBlock* m_DamageText;

public:
	bool SetDamageText(int _Damage, bool _Player)
	{
		if (m_DamageText != nullptr)
		{
			m_DamageText->SetText(FText::FromString(FString::FromInt(_Damage)));

			if (_Player)
				m_DamageText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.f, 0.f, 1.f)));

			return true;
		}

		return false;
	}
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
