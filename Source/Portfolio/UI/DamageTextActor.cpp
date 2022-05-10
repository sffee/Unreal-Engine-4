#include "DamageTextActor.h"

ADamageTextActor::ADamageTextActor()
	: m_LifeTime(0.f)
	, m_DamageTextChanged(false)
{
	PrimaryActorTick.bCanEverTick = true;

	m_DamageTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageTextWidget"));
}

void ADamageTextActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamageTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_DamageTextChanged == false)
	{
		m_DamageTextChanged = Cast<UDamageTextWidget>(m_DamageTextWidget->GetWidget())->SetDamageText(m_Damage, m_Player);
	}

	m_LifeTime += DeltaTime;
	if (0.7f <= m_LifeTime)
	{
		Destroy();
	}
	else if (0.5f <= m_LifeTime)
	{
		FVector Pos = GetActorLocation();
		Pos.Z += DeltaTime * 200.f;
		SetActorLocation(Pos);

		UDamageTextWidget* Widget = Cast<UDamageTextWidget>(m_DamageTextWidget->GetWidget());
		float Opacity = Widget->GetRenderOpacity();
		Opacity = 1 - ((m_LifeTime - 0.5f) / 0.2f);
		Widget->SetRenderOpacity(Opacity);
	}
	else
	{
		FVector Pos = GetActorLocation();
		Pos.Z += DeltaTime * 40.f;
		SetActorLocation(Pos);
	}
}