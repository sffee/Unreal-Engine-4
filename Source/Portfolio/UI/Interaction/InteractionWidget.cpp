#include "InteractionWidget.h"

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_Widget = Cast<UUserWidget>(GetWidgetFromName(TEXT("Interaction")));
	SetVisible(false);
}

void UInteractionWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}

void UInteractionWidget::SetVisible(bool _Set)
{
	if (m_Widget == nullptr)
		return;

	if (_Set)
		m_Widget->SetVisibility(ESlateVisibility::Visible);
	else
		m_Widget->SetVisibility(ESlateVisibility::Hidden);
}