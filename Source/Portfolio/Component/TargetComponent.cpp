#include "TargetComponent.h"

UTargetComponent::UTargetComponent()
	: m_Death(false)
{
	ConstructorHelpers::FClassFinder<ULockOnWidget> LockOnWidget(TEXT("WidgetBlueprint'/Game/BlueprintClass/UI/WBP_LockOn.WBP_LockOn_C'"));
	if (LockOnWidget.Succeeded())
	{
		m_LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidget"));
		m_LockOnWidgetComponent->SetupAttachment(this);

		m_LockOnWidgetComponent->SetWidgetClass(LockOnWidget.Class);

		m_LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		m_LockOnWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
	}

	SetCollisionProfileName("LockOn");
}

UTargetComponent::~UTargetComponent()
{
	int a = 0;
}

void UTargetComponent::LockOn()
{
	Cast<ULockOnWidget>(m_LockOnWidgetComponent->GetWidget())->SetLockOnVisible(true);
}

void UTargetComponent::LockOff()
{
	Cast<ULockOnWidget>(m_LockOnWidgetComponent->GetWidget())->SetLockOnVisible(false);
}