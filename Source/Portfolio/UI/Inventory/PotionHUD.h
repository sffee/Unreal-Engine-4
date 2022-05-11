#pragma once

#include "../../Etc/global.h"

#include <Components/TextBlock.h>
#include <Components/Image.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PotionHUD.generated.h"

UCLASS()
class PORTFOLIO_API UPotionHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UTextBlock* m_CountTextBlock;
	UImage* m_CooltimeProgress;
	UTextBlock* m_CooltimeTextBlock;

	UMaterialInstanceDynamic* m_Mtrl;

	float m_PotionCooltime;

public:
	void SetCount(int _Count)
	{
		m_CountTextBlock->SetText(FText::AsNumber(_Count));
	}

	bool UsePotion();
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};