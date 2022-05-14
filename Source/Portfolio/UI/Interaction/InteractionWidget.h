#pragma once

#include "../../Etc/global.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

UCLASS()
class PORTFOLIO_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UUserWidget* m_Widget;
	
public:
	void SetVisible(bool _Set);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;
};