#include "LockOnWidget.h"

void ULockOnWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_LockOnImage = Cast<UImage>(GetWidgetFromName("LockOn"));
	SetLockOnVisible(false);
}

void ULockOnWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}