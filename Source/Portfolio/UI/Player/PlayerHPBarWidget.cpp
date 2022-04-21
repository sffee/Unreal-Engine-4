#include "PlayerHPBarWidget.h"

void UPlayerHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
}

void UPlayerHPBarWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}
