#include "NormalEnemyBase.h"

#include "UI/Enemy/EnemyHPBarWidget.h"

ANormalEnemyBase::ANormalEnemyBase()
{
	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	m_WidgetComponent->SetupAttachment(GetMesh());
}

void ANormalEnemyBase::SetHP(float _Ratio)
{
	UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(m_WidgetComponent->GetWidget());
	HPBarWidget->SetHP(m_Info.CurHP / m_Info.MaxHP);
}

void ANormalEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KnockBackFlyLandCheck();
}

void ANormalEnemyBase::KnockBackFlyLandCheck()
{
	if ((m_State != EENEMY_STATE::DAMAGE_KNOCKBACK_FLY && m_State != EENEMY_STATE::DAMAGE_AIR) || m_FlyDownCheck == false)
		return;

	if (GetMovementComponent()->IsMovingOnGround())
	{
		m_Fly = false;

		if (m_Info.CurHP <= 0.f)
		{
			UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(m_WidgetComponent->GetWidget());
			HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeState(EENEMY_STATE::DEATH);
			return;
		}

		ChangeState(EENEMY_STATE::DOWN);
	}
}