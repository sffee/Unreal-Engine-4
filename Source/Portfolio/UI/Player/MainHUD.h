#pragma once

#include "../../Etc/global.h"

#include "PlayerHPBarWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

UCLASS()
class PORTFOLIO_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPlayerHPBarWidget* m_PlayerHPBarWidget;

public:
	UPlayerHPBarWidget* GetHPBarWidget()
	{
		return m_PlayerHPBarWidget;
	}

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};
