#pragma once

#include "../../Etc/global.h"

#include "PlayerHPBarWidget.h"
#include "../Inventory/PotionHUD.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

UCLASS()
class PORTFOLIO_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPlayerHPBarWidget* m_PlayerHPBarWidget;
	UPotionHUD* m_PotionHUD;

public:
	UPlayerHPBarWidget* GetHPBarWidget()
	{
		return m_PlayerHPBarWidget;
	}

	void SetPotionCount(int _Count)
	{
		m_PotionHUD->SetCount(_Count);
	}

	bool UsePotion();

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
