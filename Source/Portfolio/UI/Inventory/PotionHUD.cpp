#include "PotionHUD.h"

void UPotionHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_CountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
}

void UPotionHUD::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}