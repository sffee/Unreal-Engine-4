#include "PotionHUD.h"

void UPotionHUD::NativeConstruct()
{
	Super::NativeConstruct();

	m_CountTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Count")));
	m_CooltimeProgress = Cast<UImage>(GetWidgetFromName(TEXT("CooltimeProgress")));
	m_CooltimeTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("CooltimeText")));
	
	m_PotionCooltime = 0.f;

	m_Mtrl = m_CooltimeProgress->GetDynamicMaterial();
}

void UPotionHUD::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);

	m_PotionCooltime = 0.f < m_PotionCooltime ? m_PotionCooltime - DT : m_PotionCooltime;
	float CooltimeRatio = (10.f - m_PotionCooltime) / 10.f;
	m_Mtrl->SetScalarParameterValue(TEXT("Ratio"), CooltimeRatio);

	if (0.f < m_PotionCooltime)
	{
		FString Text = FString::Printf(TEXT("%.1f"), m_PotionCooltime);
		m_CooltimeTextBlock->SetText(FText::FromString(Text));
	}
	else
	{
		FString Text("");
		m_CooltimeTextBlock->SetText(FText::FromString(Text));
	}
}

bool UPotionHUD::UsePotion()
{
	if (0.f < m_PotionCooltime)
		return false;

	m_PotionCooltime = 10.f;
	return true;
}