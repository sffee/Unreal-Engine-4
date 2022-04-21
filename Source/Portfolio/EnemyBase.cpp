#include "EnemyBase.h"
#include "Component/TargetComponent.h"

AEnemyBase::AEnemyBase()
	: m_FlyDownCheck(false)
	, m_HitEffect(false)
	, m_Dissolve(false)
{
	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
	GetMesh()->SetCollisionProfileName("NoCollision");

	m_TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("Target"));
	m_TargetComponent->SetupAttachment(GetMesh(), TEXT("LockOn"));
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

	switch (m_State)
	{
	case EENEMY_STATE::DAMAGE_KNOCKBACK_FLY:
		m_FlyDownCheck = false;
		break;
	case EENEMY_STATE::DEATH:
		m_TargetComponent->Death();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HitEffectUpdate(DeltaTime);
	DissolveUpdate(DeltaTime);
}

void AEnemyBase::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitEffect_Ratio"), 1.f);
	m_HitEffect = true;
	m_HitEffectTimer = 0.2f;
	m_HitEffectRatio = 1.f;
}

void AEnemyBase::Dissolve()
{
	m_Dissolve = true;
	m_DissolveProgress = 0.5f;
}

void AEnemyBase::HitEffectUpdate(float _DeltaTime)
{
	if (m_HitEffect == false)
		return;

	m_HitEffectTimer -= _DeltaTime;

	if (m_HitEffectTimer <= 0.f)
	{
		//m_HitEffectRatio -= _DeltaTime * 8.f;
		//if (m_HitEffectRatio <= 0.f)
		//{
		//	m_HitEffect = false;
		//	m_HitEffectRatio = 0.f;
		//}

		GetMesh()->SetScalarParameterValueOnMaterials(TEXT("HitEffect_Ratio"), 0.f);
	}
}

void AEnemyBase::DissolveUpdate(float _DeltaTime)
{
	if (m_Dissolve == false)
		return;

	m_DissolveProgress += _DeltaTime * 0.5f;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Dissolve_Progress"), m_DissolveProgress);

	if (1.f <= m_DissolveProgress)
		Destroy();
}