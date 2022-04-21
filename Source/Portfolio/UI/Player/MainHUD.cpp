#include "MainHUD.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerHPBarWidget = Cast<UPlayerHPBarWidget>(GetWidgetFromName(TEXT("HPBar")));
}

void UMainHUD::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}