#include "Kwang.h"

AKwang::AKwang()
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
}

void AKwang::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);
}

bool AKwang::AttackCheck()
{
	return false;
}

void AKwang::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());
}

void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}