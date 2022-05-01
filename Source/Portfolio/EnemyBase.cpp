#include "EnemyBase.h"

#include "Trigger/EnemySpawnTrigger.h"

#include "Manager/Effectmanager.h"

#include "Enemy/EnemyAIController.h"
#include "Component/TargetComponent.h"
#include "Player/MyPlayer.h"

AEnemyBase::AEnemyBase()
	: m_FlyDownCheck(false)
	, m_Dissolve(false)
	, m_Attack(false)
	, m_Fly(false)
	, m_LoopAttackCheck(false)
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName("Enemy");
	GetMesh()->SetCollisionProfileName("NoCollision");

	m_TargetComponent = CreateDefaultSubobject<UTargetComponent>(TEXT("Target"));
	m_TargetComponent->SetupAttachment(GetMesh(), TEXT("LockOn"));

	JumpMaxCount = 2;
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

	PlayMontage(m_State);
	LookToPlayer();
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

	if (EENEMY_STATE::ATTACK1 <= m_State && m_State <= EENEMY_STATE::ATTACK6)
		m_Attack = true;

	switch (m_State)
	{
	case EENEMY_STATE::IDLE:
	case EENEMY_STATE::RUN:
		m_Damage = false;
		m_Attack = false;
		break;
	case EENEMY_STATE::DAMAGE_KNOCKBACK_FLY:
		m_FlyDownCheck = false;
	case EENEMY_STATE::DAMAGE_KNOCKBACK_GROUND:
		m_Damage = true;
		break;
	case EENEMY_STATE::DEATH:
		if (Die_Delegate.IsBound())
			Die_Delegate.Execute();

		m_TargetComponent->Death();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveUpdate(DeltaTime);
	CooltimeUpdate(DeltaTime);
}

void AEnemyBase::CooltimeUpdate(float _DeltaTime)
{
	for (int i = 0; i < 6; i++)
	{
		m_RemainAttackCooltime[i] -= _DeltaTime;
		if (m_RemainAttackCooltime[i] < 0.f)
			m_RemainAttackCooltime[i] = 0.f;
	}
}

void AEnemyBase::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);
}

void AEnemyBase::Dissolve()
{
	m_Dissolve = true;
	m_DissolveProgress = 0.5f;
}

void AEnemyBase::LookToPlayer()
{
	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	FVector TargetDir = Player->GetActorLocation() - GetActorLocation();
	FRotator TargetRot = TargetDir.GetSafeNormal().Rotation();
	FRotator MyRot = GetControlRotation();
	MyRot.Pitch = 0.f;
	MyRot.Yaw = TargetRot.Yaw;

	SetActorRotation(MyRot);
}

FRotator AEnemyBase::GetRotationToPlayer()
{
	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

	FVector TargetDir = Player->GetActorLocation() - GetActorLocation();
	FRotator TargetRot = TargetDir.GetSafeNormal().Rotation();
	
	return TargetRot;
}

void AEnemyBase::DissolveUpdate(float _DeltaTime)
{
	if (m_Dissolve == false)
		return;

	m_DissolveProgress += _DeltaTime * 0.3f;

	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("Dissolve_Progress"), m_DissolveProgress);

	if (1.f <= m_DissolveProgress)
		Destroy();
}

void AEnemyBase::Attack()
{
	FName RowName = m_MontageMap.FindRef(m_State);

	FEnemyMontageInfo* MontageInfo = m_MontageTable->FindRow<FEnemyMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
	{
		FName Section = m_AnimInst->Montage_GetCurrentSection(MontageInfo->Montage);
		FAttackInfo* AttackInfo = m_AttackInfoTable->FindRow<FAttackInfo>(Section, TEXT(""));
		if (AttackInfo != nullptr)
		{
			FVector vPos = GetActorLocation() + (GetActorForwardVector() * AttackInfo->XPivot);

			TArray<FHitResult> arrHit;
			FCollisionQueryParams param(NAME_None, false, this);

			GetWorld()->SweepMultiByChannel(arrHit, vPos, vPos, FQuat::Identity
				, ECC_GameTraceChannel8
				, FCollisionShape::MakeSphere(AttackInfo->Radius), param);

			bool HitSuccess = false;

			if (arrHit.Num())
			{
				for (size_t i = 0; i < arrHit.Num(); ++i)
				{
					if (m_LoopAttackCheck)
					{
						if (m_AttackHitActors.Find(arrHit[i].Actor.Get()) != INDEX_NONE)
							continue;
					}

					if (HitProcess(arrHit[i], AttackInfo))
					{
						HitSuccess = true;

						if (m_LoopAttackCheck)
							m_AttackHitActors.AddUnique(arrHit[i].Actor.Get());
					}
				}
			}

#ifdef ENABLE_DRAW_DEBUG
			//FColor color = HitSuccess ? FColor::Green : FColor::Red;
			//DrawDebugSphere(GetWorld(), vPos, AttackInfo->Radius, 20, color, false, 0.1f);
#endif
		}
	}
}

bool AEnemyBase::HitProcess(const FHitResult& _HitResult, const FAttackInfo* _AttackInfo)
{
	bool Return = false;

	FVector ForwardVec = GetActorForwardVector();
	ForwardVec.Z = 0.f;
	ForwardVec.Normalize();

	FVector DirVec = _HitResult.Actor->GetActorLocation() - GetActorLocation();
	DirVec.Z = 0.f;
	DirVec.Normalize();

	float Dot = FVector::DotProduct(ForwardVec, DirVec);
	float Radian = FMath::Acos(Dot);
	float Angle = FMath::Abs(FMath::RadiansToDegrees(Radian));

	if (Angle <= _AttackInfo->Angle / 2)
	{
		AMyPlayer* Player = Cast<AMyPlayer>(_HitResult.Actor);
		if (Player != nullptr)
		{
			float Radius = Player->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.f;
			float Height = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f;
			float XRandom = FMath::RandRange(-Radius, Radius);
			float ZRandom = FMath::RandRange(-Height, Height);
			FTransform Trans(Player->GetMesh()->GetBoneLocation(TEXT("spine_01")) + FVector(XRandom, 0.f, ZRandom));

			UObject* Object = ULevelStreamManager::GetInst(GetWorld())->FindAsset(FName(_AttackInfo->HitEffect->GetPathName()));
			UEffectManager::GetInst(GetWorld())->CreateEffect(Object, Trans, GetLevel(), _AttackInfo->HitEffectScale);

			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(_AttackInfo->CameraShake);
			SlowTime(_AttackInfo->SlowPower, _AttackInfo->SlowTime);

			Player->Damage(this, _AttackInfo);
			Return = true;
		}
	}

	return Return;
}