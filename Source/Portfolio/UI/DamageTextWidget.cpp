#include "DamageTextWidget.h"

void UDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_DamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("DamageText")));
}

void UDamageTextWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}