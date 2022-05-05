#include "Kwang.h"

#include "../Player/MyPlayer.h"
#include "../Component/TargetComponent.h"

#include <BehaviorTree/BlackboardComponent.h>
#include "EnemyAIController.h"

AKwang::AKwang()
	: m_WaitTime(0.f)
{
	m_Info.MaxHP = 2000.f;
	m_Info.CurHP = 2000.f;

	ConstructorHelpers::FObjectFinder<UBlackboardData> Blackboard(TEXT("BlackboardData'/Game/BlueprintClass/Enemy/Kwang/BB_Kwang.BB_Kwang'"));
	if (Blackboard.Succeeded())
		m_Blackboard = Blackboard.Object;

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree(TEXT("BehaviorTree'/Game/BlueprintClass/Enemy/Kwang/BT_Kwang.BT_Kwang'"));
	if (BehaviorTree.Succeeded())
		m_BehaviorTree = BehaviorTree.Object;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC'"));
	if (EnemyMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(EnemyMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Kwang/ABP_Kwang.ABP_Kwang_C'"));
	if (AnimBlueprint.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);

	ConstructorHelpers::FObjectFinder<UDataTable> MontageTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Kwang/DataTable/KwangMontageTable.KwangMontageTable'"));
	if (MontageTable.Succeeded())
		m_MontageTable = MontageTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> AttackInfoTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Kwang/DataTable/KwangAttackInfoTable.KwangAttackInfoTable'"));
	if (AttackInfoTable.Succeeded())
		m_AttackInfoTable = AttackInfoTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> AttackCooltime(TEXT("DataTable'/Game/BlueprintClass/Enemy/Kwang/DataTable/KwangAttackCooltime.KwangAttackCooltime'"));
	if (AttackCooltime.Succeeded())
		m_AttackCooltimeTable = AttackCooltime.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> AttackMoveTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Kwang/DataTable/KwangAttackMoveTable.KwangAttackMoveTable'"));
	if (AttackMoveTable.Succeeded())
		m_AttackMoveTable = AttackMoveTable.Object;
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();

	UBlackboardComponent* BB = Cast<AEnemyAIController>(Controller)->GetBlackboardComponent();
	BB->SetValueAsFloat(TEXT("Attack1Cooltime"), 0.f);
	BB->SetValueAsFloat(TEXT("Attack2Cooltime"), 0.f);
	BB->SetValueAsBool(TEXT("Attacking"), false);
}

void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StateUpdate();
	Attack4Update();
}

void AKwang::StateUpdate()
{
	if (m_State == EENEMY_STATE::ATTACK4)
	{
		FName RowName = m_MontageMap.FindRef(m_State);
		FEnemyMontageInfo* MontageInfo = m_MontageTable->FindRow<FEnemyMontageInfo>(RowName, TEXT(""));
		if (MontageInfo != nullptr)
		{
			FName Section = m_AnimInst->Montage_GetCurrentSection(MontageInfo->Montage);
			if (Section == "Attack4_Start" || Section == "Attack4_Charge")
				LookToPlayer();
		}
	}
}

void AKwang::Attack4Update()
{
	if (m_State == EENEMY_STATE::ATTACK4)
	{
		FName RowName = m_MontageMap.FindRef(m_State);
		FEnemyMontageInfo* MontageInfo = m_MontageTable->FindRow<FEnemyMontageInfo>(RowName, TEXT(""));
		if (MontageInfo != nullptr)
		{
			FName Section = m_AnimInst->Montage_GetCurrentSection(MontageInfo->Montage);
			if (Section == "Attack4_Dash")
			{
				FVector vPos = GetActorLocation() + (GetActorForwardVector() * 200.f);

				TArray<FHitResult> arrHit;
				FCollisionQueryParams param(NAME_None, false, this);

				GetWorld()->SweepMultiByChannel(arrHit, vPos, vPos, FQuat::Identity
					, ECC_GameTraceChannel8
					, FCollisionShape::MakeSphere(200.f), param);

				if (arrHit.Num())
				{
					ChangeState(EENEMY_STATE::ATTACK5);
				}
			}
		}
	}
}

void AKwang::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);
}