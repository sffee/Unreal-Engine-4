#include "EnemyHPBarWidget.h"

void UEnemyHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
}

void UEnemyHPBarWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}