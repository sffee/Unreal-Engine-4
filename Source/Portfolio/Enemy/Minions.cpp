#include "Minions.h"

#include "../Player/MyPlayer.h"
#include "../Component/TargetComponent.h"

#include "../UI/Enemy/EnemyHPBarWidget.h"

AMinions::AMinions()
{
	m_Info.MaxHP = 200.f;
	m_Info.CurHP = 200.f;

	ConstructorHelpers::FObjectFinder<UBlackboardData> Blackboard(TEXT("BlackboardData'/Game/BlueprintClass/Enemy/Minions/BB_Minions.BB_Minions'"));
	if (Blackboard.Succeeded())
		m_Blackboard = Blackboard.Object;

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/BlueprintClass/Enemy/Minions/BT_Minions.BT_Minions'"));
	if (BehaviorTree.Succeeded())
		m_BehaviorTree = BehaviorTree.Object;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Core_Dawn.Minion_Lane_Melee_Core_Dawn'"));
	if (EnemyMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(EnemyMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Minions/ABP_Minions.ABP_Minions_C'"));
	if (AnimBlueprint.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);

	ConstructorHelpers::FObjectFinder<UDataTable> MontageTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Minions/DataTable/MinionsMontageTable.MinionsMontageTable'"));
	if (MontageTable.Succeeded())
		m_MontageTable = MontageTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> AttackInfoTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Minions/DataTable/MinionsAttackInfoTable.MinionsAttackInfoTable'"));
	if (AttackInfoTable.Succeeded())
		m_AttackInfoTable = AttackInfoTable.Object;
}

void AMinions::BeginPlay()
{
	Super::BeginPlay();

	FVector Pos = GetActorLocation();
	Pos.Z += 90.f;
	SetActorLocation(Pos);

	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}

void AMinions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpawnUpdate();
}

void AMinions::SpawnUpdate()
{
	if (m_State != EENEMY_STATE::SPAWN)
		return;

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		ChangeState(EENEMY_STATE::SPAWN_LANDING);
	}
}

void AMinions::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	if (m_Fly == false && m_Info.CurHP <= 0.f)
		return;

	Super::Damage(_Actor, _AttackInfo);

	m_Info.CurHP -= _AttackInfo->Damage;
	SetHP(m_Info.CurHP / m_Info.MaxHP);

	FRotator Rotator = _Actor->GetActorRotation();
	Rotator.Roll = 0.f;
	Rotator.Pitch = 0.f;
	Rotator.Yaw += 180.f;
	SetActorRotation(Rotator);

	FVector HitVector = _Actor->GetActorForwardVector();
	HitVector.X *= _AttackInfo->KnockBackPowerXY;
	HitVector.Y *= _AttackInfo->KnockBackPowerXY;
	HitVector.Z = _AttackInfo->KnockBackPowerZ;

	GetCharacterMovement()->Velocity = FVector::ZeroVector;

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
			UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(m_WidgetComponent->GetWidget());
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
			UEnemyHPBarWidget* HPBarWidget = Cast<UEnemyHPBarWidget>(m_WidgetComponent->GetWidget());
			HPBarWidget->SetVisibility(ESlateVisibility::Hidden);
			ChangeState(EENEMY_STATE::DEATH);
			return;
		}

		LaunchCharacter(HitVector, false, false);
		ChangeState(EENEMY_STATE::DAMAGE_KNOCKBACK_GROUND, true);
	}
}

bool AMinions::AttackCheck()
{
	if (IsDamage() || m_State == EENEMY_STATE::DEATH)
		return false;

	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	FVector PlayerPos = Player->GetActorLocation();
	PlayerPos.Z = GetActorLocation().Z;
	float Distance = FVector::Distance(GetActorLocation(), PlayerPos);

	if (Distance <= 200.f)
	{
		m_Attack = true;
		LookToPlayer();
		ChangeState(EENEMY_STATE::ATTACK1);
		return true;
	}

	return false;
}