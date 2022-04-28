#include "Murdock.h"

#include "../Player/MyPlayer.h"
#include "../UI/Enemy/EnemyHPBarWidget.h"
#include "../Component/TargetComponent.h"

#include "../Manager/LevelStreamManager.h"

AMurdock::AMurdock()
	: m_AttackDistance(1000.f)
{
	m_WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	m_WidgetComponent->SetupAttachment(GetMesh());

	m_Info.MaxHP = 200.f;
	m_Info.CurHP = 200.f;

	ConstructorHelpers::FObjectFinder<UBlackboardData> Blackboard(TEXT("BlackboardData'/Game/BlueprintClass/Enemy/Murdock/BB_Murdock.BB_Murdock'"));
	if (Blackboard.Succeeded())
		m_Blackboard = Blackboard.Object;

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/BlueprintClass/Enemy/Murdock/BT_Murdock.BT_Murdock'"));
	if (BehaviorTree.Succeeded())
		m_BehaviorTree = BehaviorTree.Object;
		
	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'"));
	if (EnemyMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(EnemyMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Murdock/ABP_Murdock.ABP_Murdock_C'"));
	if (AnimBlueprint.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);

	ConstructorHelpers::FObjectFinder<UDataTable> MontageTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Murdock/DataTable/MurdockMontageTable.MurdockMontageTable'"));
	if (MontageTable.Succeeded())
		m_MontageTable = MontageTable.Object;

	ConstructorHelpers::FClassFinder<AProjectile> AttackProjectile(TEXT("Blueprint'/Game/BlueprintClass/Enemy/Murdock/BP_Bullet.BP_Bullet_C'"));
	if (AttackProjectile.Succeeded())
		m_AttackProjectile = AttackProjectile.Class;
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

void AMurdock::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	if (m_Fly == false && m_Info.CurHP <= 0.f)
		return;

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

	if (m_Fly == true)
	{
		if (HitVector.Z == 0.f)
		{
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->GravityScale = 0.f;
		}
		else
		{
			GetCharacterMovement()->GravityScale = 1.f;
		}

		LaunchCharacter(HitVector, false, false);
		ChangeState(EENEMY_STATE::DAMAGE_AIR, true);
	}
	else if (0.f < _AttackInfo->KnockBackPowerZ)
	{
		if (m_Info.CurHP <= 0.f)
		{
			HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
			m_TargetComponent->Death();

			if (_AttackInfo->KnockBackPowerXY == 0.f)
			{
				ChangeState(EENEMY_STATE::DEATH);
				return;
			}
		}

		m_Fly = true;
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

bool AMurdock::Attack()
{
	if (IsDamage() || m_State == EENEMY_STATE::DEATH)
		return false;

	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	float Distance = FVector::Distance(GetActorLocation(), Player->GetActorLocation());

	if (Distance <= m_AttackDistance)
	{
		m_Attack = true;
		LookToPlayer();
		ChangeState(EENEMY_STATE::ATTACK1);
		return true;
	}

	return false;
}

void AMurdock::Fire()
{
	FVector Pos = GetActorLocation();
	FRotator Rot = GetActorRotation();
	FVector Velocity = GetActorForwardVector();
	SpawnProjectile(m_AttackProjectile, Pos, Rot, Velocity);
}