#pragma once

#include "../../Etc/global.h"

#include <Components/TextBlock.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PotionHUD.generated.h"

UCLASS()
class PORTFOLIO_API UPotionHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UTextBlock* m_CountTextBlock;

public:
	void SetCount(int _Count)
	{
		m_CountTextBlock->SetText(FText::AsNumber(_Count));
	}
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};