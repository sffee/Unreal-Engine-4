#include "Murdock.h"
#include "../Player/MyPlayer.h"
#include "../UI/Enemy/EnemyHPBarWidget.h"

AMurdock::AMurdock()
{
	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	m_WidgetComponent->SetupAttachment(GetMesh());

	m_Info.MaxHP = 200.f;
	m_Info.CurHP = 200.f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'"));
	if (EnemyMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(EnemyMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Murdock/ABP_Murdock.ABP_Murdock_C'"));
	if (AnimBlueprint.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);

	ConstructorHelpers::FObjectFinder<UDataTable> MontageTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Murdock/DataTable/MurdockMontageTable.MurdockMontageTable'"));
	if (MontageTable.Succeeded())
		m_MontageTable = MontageTable.Object;
}

void AMurdock::BeginPlay()
{
	Super::BeginPlay();
}

void AMurdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KnockBackFlyLandCheck();
}

void AMurdock::KnockBackFlyLandCheck()
{
	if (m_State != EENEMY_STATE::DAMAGE_KNOCKBACK_FLY || m_FlyDownCheck == false)
		return;

	if (GetMovementComponent()->IsMovingOnGround())
	{
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

void AMurdock::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);

	m_Info.CurHP -= _AttackInfo->Damage;

	UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(m_WidgetComponent->GetWidget());
	HPBarWidget->SetHP(m_Info.CurHP / m_Info.MaxHP);

	FRotator Rotator = _Actor->GetActorRotation();
	Rotator.Roll = 0.f;
	Rotator.Pitch = 0.f;
	Rotator.Yaw += 180.f;
	SetActorRotation(Rotator);

	FVector HitVector = _Actor->GetActorForwardVector();
	HitVector.X *= _AttackInfo->KnockBackPowerXY;
	HitVector.Y *= _AttackInfo->KnockBackPowerXY;
	HitVector.Z = _AttackInfo->KnockBackPowerZ;

	if (0.f < _AttackInfo->KnockBackPowerZ)
	{
		if (m_Info.CurHP <= 0.f)
			HPBarWidget->SetVisibility(ESlateVisibility::Hidden);

		LaunchCharacter(HitVector, false, false);
		ChangeState(EENEMY_STATE::DAMAGE_KNOCKBACK_FLY);
	}
	else
	{
		if (m_Info.CurHP <= 0.f)
		{
			HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeState(EENEMY_STATE::DEATH);
			return;
		}

		LaunchCharacter(HitVector, false, false);
		ChangeState(EENEMY_STATE::DAMAGE_KNOCKBACK_GROUND, true);
	}
}