#include "EnemyBase.h"
#include "Component/TargetComponent.h"

AEnemyBase::AEnemyBase()
	: m_FlyDownCheck(false)
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> EnemyMesh(TEXT("SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'"));
	if (EnemyMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(EnemyMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/BlueprintClass/Enemy/Murdock/ABP_Murdock.ABP_Murdock_C'"));
	if (AnimBlueprint.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);

	ConstructorHelpers::FObjectFinder<UDataTable> MontageTable(TEXT("DataTable'/Game/BlueprintClass/Enemy/Murdock/DataTable/MurdockMontageTable.MurdockMontageTable'"));
	if (MontageTable.Succeeded())
		m_MontageTable = MontageTable.Object;

	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
	UTargetComponent* TargetCom = CreateDefaultSubobject<UTargetComponent>(TEXT("Target"));
	TargetCom->SetupAttachment(GetMesh(), TEXT("LockOn"));
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	FString Str;
	TArray<FEnemyMontageInfo*> AllRows;
	m_MontageTable->GetAllRows<FEnemyMontageInfo>(Str, AllRows);

	TArray<FName> AllRowNames = m_MontageTable->GetRowNames();;

	for (int i = 0; i < AllRows.Num(); i++)
		m_MontageMap.Add(AllRows[i]->State, AllRowNames[i]);

	PlayMontage(EENEMY_STATE::IDLE);
}

void AEnemyBase::PlayMontage(EENEMY_STATE _State)
{
	FName RowName = m_MontageMap.FindRef(_State);

	FEnemyMontageInfo* MontageInfo = m_MontageTable->FindRow<FEnemyMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
		m_AnimInst->Montage_Play(MontageInfo->Montage);
}

void AEnemyBase::ChangeState(EENEMY_STATE _NextState, bool _Ignore)
{
	if (_Ignore == false && m_State == _NextState)
		return;

	m_State = _NextState;

	PlayMontage(m_State);

	if (m_State == EENEMY_STATE::DAMAGE_KNOCKBACK_FLY)
	{
		m_FlyDownCheck = false;
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyBase::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);
}