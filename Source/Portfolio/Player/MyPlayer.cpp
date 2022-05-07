// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"
#include "../EnemyBase.h"
#include "../Manager/EffectManager.h"
#include "../Component/LockOnArmComponent.h"
#include "../Component/TargetComponent.h"
#include "../PortfolioGameModeBase.h"

AMyPlayer::AMyPlayer()
	: m_State(EPLAYER_STATE::SWORD_IDLE_L)
	, m_Moveable(true)
	, m_Attacking(false)
	, m_AttackCancleable(true)
	, m_ComboBCount(0)
	, m_PressWKey(false)
	, m_PressSKey(false)
	, m_PressAKey(false)
	, m_PressDKey(false)
	, m_PressSpecialAttackKey(false)
	, m_PressLockOn(false)
	, m_Jump(false)
	, m_JumpSecond(false)
	, m_JumpAttack(false)
	, m_JumpAttackB(false)
	, m_DashAttackTimer(0.f)
	, m_LoopAttackCheck(false)
{
	m_LockOnArm = CreateDefaultSubobject<ULockOnArmComponent>(TEXT("LockOnArm"));
	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	m_LockOnArm->SetupAttachment(GetMesh());
	m_Cam->SetupAttachment(m_LockOnArm);

	m_Info.WeaponType = EWEAPON_TYPE::SWORD;
	m_Info.CurHP = 200.f;
	m_Info.MaxHP = 200.f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMesh(TEXT("SkeletalMesh'/Game/Player/Character/Meshes/Player.Player'"));
	if (PlayerMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(PlayerMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBlueprint(TEXT("AnimBlueprint'/Game/BlueprintClass/Player/ABP_Player.ABP_Player_C'"));
	if (AnimBlueprint.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimBlueprint.Class);

	ConstructorHelpers::FObjectFinder<UDataTable> MontageTable(TEXT("DataTable'/Game/BlueprintClass/Player/DataTable/PlayerMontageTable.PlayerMontageTable'"));
	if (MontageTable.Succeeded())
		m_MontageTable = MontageTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> AttackMoveTable(TEXT("DataTable'/Game/BlueprintClass/Player/DataTable/PlayerAttackMoveTable.PlayerAttackMoveTable'"));
	if (AttackMoveTable.Succeeded())
		m_AttackMoveTable = AttackMoveTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> AttackInfoTable(TEXT("DataTable'/Game/BlueprintClass/Player/DataTable/PlayerAttackInfoTable.PlayerAttackInfoTable'"));
	if (AttackInfoTable.Succeeded())
		m_AttackInfoTable = AttackInfoTable.Object;
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	TableSetting();

	PlayMontage(EPLAYER_STATE::SWORD_IDLE_L);

	APlayerCameraManager* CameraManager = Cast<APlayerController>(Controller)->PlayerCameraManager;
	CameraManager->ViewPitchMax = 10.f;
	CameraManager->ViewPitchMin = -30.f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPlayer::OnBeginOverlap);

	m_EtcMesh.Add(Cast<USkeletalMeshComponent>(GetDefaultSubobjectByName(TEXT("Wing"))));
	m_EtcMesh.Add(Cast<USkeletalMeshComponent>(GetDefaultSubobjectByName(TEXT("Sword"))));

	JumpMaxCount = 3;
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString a = FString::Printf(TEXT("%f %f %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	LOG(Log, *a);

	HUDUpdate();
	JumpUpdate();
	CheckRunAnimation();
	DashAttackUpdate(DeltaTime);
	LockOnCameraUpdate(DeltaTime);
	LoopAttackUpdate();
}

void AMyPlayer::LockOnCameraUpdate(float _DeltaTime)
{
	if (m_LockOnArm->IsLockOn() == false)
		return;

	if (m_PressLockOn == true)
	{
		float ElapsedTime = GetWorld()->GetRealTimeSeconds() - m_PressLockOnTime;
		if (m_LockOnArm->GetLockOffPressTime() <= ElapsedTime)
		{
			m_PressLockOn = false;
			m_LockOnArm->LockOff();
			return;
		}
	}

	FVector TargetDir = m_LockOnArm->GetLockOnTarget()->GetComponentLocation() - m_LockOnArm->GetComponentLocation();
	FRotator TargetRot = TargetDir.GetSafeNormal().Rotation();
	TargetRot.Pitch = 340.f;
	FRotator MyRot = GetControlRotation();
	FRotator NewRot = FMath::RInterpTo(MyRot, TargetRot, _DeltaTime, 5.f);

	GetController()->SetControlRotation(NewRot);
}

void AMyPlayer::HUDUpdate()
{
	APortfolioGameModeBase* GM = Cast<APortfolioGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	float HPRatio = m_Info.CurHP / m_Info.MaxHP;
	GM->UpdateHPBar(HPRatio);
}

void AMyPlayer::JumpUpdate()
{
	if (m_Jump == true)
	{
		if (GetMovementComponent()->IsMovingOnGround())
		{
			if (m_State == EPLAYER_STATE::JUMP_LOOP)
				ChangeState(EPLAYER_STATE::JUMP_LANDING);
			else if (m_State == EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_LOOP)
				ChangeState(EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_END);
			else if (m_State == EPLAYER_STATE::JUMP_DAMAGE_LOOP)
				ChangeState(EPLAYER_STATE::JUMP_DAMAGE_LANDING);
		}
		else
		{
			if (m_State == EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_LOOP)
				GetCharacterMovement()->Velocity = FVector(0.f, 0.f, -2500.f);
		}

	}
}

void AMyPlayer::DashAttackUpdate(float _DeltaTime)
{
	if (EPLAYER_STATE::SWORD_DASHATTACK_START <= m_State && m_State <= EPLAYER_STATE::SWORD_DASHATTACK_RUN_LOOP)
	{
		FVector vPos = GetActorLocation() + (GetActorForwardVector() * 200.f);

		TArray<FHitResult> arrHit;
		FCollisionQueryParams param(NAME_None, false, this);

		GetWorld()->SweepMultiByChannel(arrHit, vPos, vPos, FQuat::Identity
			, ECC_GameTraceChannel3
			, FCollisionShape::MakeSphere(100.f), param);

		if (arrHit.Num())
		{
			ChangeState(EPLAYER_STATE::SWORD_DASHATTACK_FINISH_START);
			return;
		}
	}

	if (m_State == EPLAYER_STATE::SWORD_DASHATTACK_RUN_LOOP)
	{
		m_DashAttackTimer += _DeltaTime;
		if (0.5f <= m_DashAttackTimer)
		{
			ChangeState(EPLAYER_STATE::SWORD_DASHATTACK_FINISH_START);
		}
	}
}

void AMyPlayer::LoopAttackUpdate()
{
	if (m_LoopAttackCheck == false)
		return;

	Attack();
}

void AMyPlayer::TableSetting()
{
	FString Str;
	TArray<FPlayerMontageInfo*> AllRows;
	m_MontageTable->GetAllRows<FPlayerMontageInfo>(Str, AllRows);

	TArray<FName> AllRowNames = m_MontageTable->GetRowNames();

	for (int i = 0; i < AllRows.Num(); i++)
		m_MontageMap.Add(AllRows[i]->State, AllRowNames[i]);
}

void AMyPlayer::PlayMontage(EPLAYER_STATE _State)
{
	FName RowName = m_MontageMap.FindRef(_State);

	FPlayerMontageInfo* MontageInfo = m_MontageTable->FindRow<FPlayerMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
	{
		m_AnimInst->Montage_Play(MontageInfo->Montage);
		GetCharacterMovement()->bUseSeparateBrakingFriction = MontageInfo->UseSeparateBrakingFriction;
		GetCharacterMovement()->GravityScale = MontageInfo->Gravity;
	}
}

void AMyPlayer::LookToLockOnTarget()
{
	if (m_LockOnArm->IsLockOn() == false)
		return;

	FVector TargetDir = m_LockOnArm->GetLockOnTarget()->GetComponentLocation() - m_LockOnArm->GetComponentLocation();
	FRotator TargetRot = TargetDir.GetSafeNormal().Rotation();
	FRotator MyRot = GetControlRotation();
	MyRot.Pitch = 0.f;
	MyRot.Yaw = TargetRot.Yaw;

	SetActorRotation(MyRot);
}

void AMyPlayer::CheckRunAnimation()
{
	if (m_Attacking && IsPressMoveKey() && m_Moveable == true)
	{
		ChangeState(EPLAYER_STATE::SWORD_RUN);
		return;
	}

	if ((m_State == EPLAYER_STATE::SWORD_IDLE_L || m_State == EPLAYER_STATE::SWORD_IDLE_R || m_State == EPLAYER_STATE::SWORD_RUN_END || m_State == EPLAYER_STATE::DAMAGE || m_State == EPLAYER_STATE::EVADE || m_State == EPLAYER_STATE::JUMP_LANDING)
		&& IsPressMoveKey() && m_Moveable == true)
	{
		ChangeState(EPLAYER_STATE::SWORD_RUN);
		return;
	}

	if (m_State == EPLAYER_STATE::SWORD_RUN
		&& IsPressMoveKey() == false)
	{
		ChangeState(EPLAYER_STATE::SWORD_RUN_END);
		return;
	}
}

void AMyPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &AMyPlayer::MoveSide);
	PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &AMyPlayer::MoveFront);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyPlayer::LookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayer::JumpAction);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayer::AttackAction);
	PlayerInputComponent->BindAction(TEXT("AttackR"), EInputEvent::IE_Pressed, this, &AMyPlayer::AttackRAction);
	PlayerInputComponent->BindAction(TEXT("Evade"), EInputEvent::IE_Pressed, this, &AMyPlayer::EvadeAction);

	PlayerInputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Pressed, this, &AMyPlayer::LockOnDownAction);
	PlayerInputComponent->BindAction(TEXT("LockOn"), EInputEvent::IE_Released, this, &AMyPlayer::LockOnUpAction);

	PlayerInputComponent->BindAction(TEXT("SpecialAttack"), EInputEvent::IE_Pressed, this, &AMyPlayer::SpecialAttackDownAction);
	PlayerInputComponent->BindAction(TEXT("SpecialAttack"), EInputEvent::IE_Released, this, &AMyPlayer::SpecialAttackUpAction);
}

void AMyPlayer::ChangeState(EPLAYER_STATE _NextState, bool _Ignore)
{
	if (_Ignore == false && m_State == _NextState)
	{
		return;
	}

	m_State = _NextState;

	m_Moveable = true;

	if (EPLAYER_STATE::SWORD_COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::SWORD_UPPER_JUMP)
	{
		m_Attacking = true;
		m_Moveable = false;
		m_AttackCancleable = false;
	}
	else if (EPLAYER_STATE::SWORD_DASHATTACK_START <= m_State && m_State <= EPLAYER_STATE::SWORD_DASHATTACK_FINISH_END)
	{
		m_Attacking = true;
		m_Moveable = false;
		m_AttackCancleable = false;
	}
	else
	{
		m_Attacking = false;
		SetAttackMove(false);
		m_AttackCancleable = true;
	}

	if (_NextState == EPLAYER_STATE::EVADE)
		m_AttackCancleable = false;

	if (m_State != _NextState)
	{
		if (m_State == EPLAYER_STATE::EVADE)
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	}

	PlayMontage(m_State);

	switch (m_State)
	{
	case EPLAYER_STATE::SWORD_IDLE_L:
	case EPLAYER_STATE::SWORD_IDLE_R:
		m_Attacking = false;
		m_Moveable = true;
		SetAttackMove(false);
		m_AttackCancleable = true;
		SetDamage(false);
		break;
	case EPLAYER_STATE::SWORD_RUN:
		GetCharacterMovement()->MaxWalkSpeed = 4000.f;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		m_Attacking = false;
		m_Moveable = true;
		SetAttackMove(false);
		m_AttackCancleable = true;
		SetDamage(false);
		break;
	case EPLAYER_STATE::SWORD_RUN_END:
		GetCharacterMovement()->Velocity /= 1.3f;
		break;
	case EPLAYER_STATE::DAMAGE:
		m_Damage = true;
		m_Moveable = false;
		break;
	case EPLAYER_STATE::EVADE:
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		m_Moveable = false;
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player_Evade"));
		break;
	case EPLAYER_STATE::JUMP_SECOND:
		m_JumpSecond = true;
	case EPLAYER_STATE::JUMP:
	case EPLAYER_STATE::JUMP_LOOP:
		m_Jump = true;
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		m_Moveable = false;
		break;
	case EPLAYER_STATE::JUMP_LANDING:
	case EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_END:
	case EPLAYER_STATE::JUMP_DAMAGE_LANDING:
		m_Jump = false;
		m_JumpSecond = false;
		m_JumpAttack = false;
		m_JumpAttackB = false;
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		m_Moveable = false;
		break;
	case EPLAYER_STATE::SWORD_DASHATTACK_FINISH_START:
		GetCharacterMovement()->Velocity /= 3.f;
		break;
	default:
		break;
	}
}

void AMyPlayer::Attack()
{
	FName RowName = m_MontageMap.FindRef(m_State);

	FPlayerMontageInfo* MontageInfo = m_MontageTable->FindRow<FPlayerMontageInfo>(RowName, TEXT(""));
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
				, ECC_GameTraceChannel3
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

void AMyPlayer::Upper()
{
	if (m_PressSpecialAttackKey == false)
	{
		ChangeState(EPLAYER_STATE::SWORD_UPPER);
	}
	else
	{
		GetCharacterMovement()->JumpZVelocity = 1300.f;
		Jump();
		m_Jump = true;
		m_JumpAttack = false;
		m_JumpAttackB = false;
		ChangeState(EPLAYER_STATE::SWORD_UPPER_JUMP);
	}
}

bool AMyPlayer::HitProcess(const FHitResult& _HitResult, const FAttackInfo* _AttackInfo)
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
		AEnemyBase* Enemy = Cast<AEnemyBase>(_HitResult.Actor);
		if (Enemy != nullptr)
		{
			float Radius = Enemy->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.f;
			float Height = Enemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f;
			float YRandom = FMath::RandRange(-Radius, Radius);
			float ZRandom = FMath::RandRange(-Height, Height);
			FTransform Trans(Enemy->GetMesh()->GetSocketLocation(TEXT("HitEffect")) + FVector(0.f, YRandom, ZRandom));

			UObject* Object = ULevelStreamManager::GetInst(GetWorld())->FindAsset(FName(_AttackInfo->HitEffect->GetPathName()));
			UEffectManager::GetInst(GetWorld())->CreateEffect(Object, Trans, GetLevel(), _AttackInfo->HitEffectScale);

			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(_AttackInfo->CameraShake);
			SlowTime(_AttackInfo->SlowPower, _AttackInfo->SlowTime);

			Enemy->Damage(this, _AttackInfo);
			Return = true;
		}
	}

	return Return;
}

void AMyPlayer::AttackMoveSpeedSetting(EPLAYER_STATE _State)
{
	FName RowName = m_MontageMap.FindRef(_State);

	FPlayerMontageInfo* MontageInfo = m_MontageTable->FindRow<FPlayerMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
	{
		FName Section = m_AnimInst->Montage_GetCurrentSection(MontageInfo->Montage);
		FAttackMoveInfo* AttackMoveInfo = m_AttackMoveTable->FindRow<FAttackMoveInfo>(Section, TEXT(""));
		if (AttackMoveInfo != nullptr)
		{
			GetMovementComponent()->StopMovementImmediately();

			GetCharacterMovement()->Velocity = GetActorForwardVector() * AttackMoveInfo->StartSpeed;
			GetCharacterMovement()->MaxWalkSpeed = AttackMoveInfo->MaxWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = AttackMoveInfo->MaxAcceleration;
		}
	}
}

void AMyPlayer::MoveFront(float _Scale)
{
	if (_Scale == 1.f)
		m_PressWKey = true;
	else if (_Scale == -1.f)
		m_PressSKey = true;
	else
	{
		m_PressWKey = false;
		m_PressSKey = false;
	}

	if (m_Moveable == false)
		return;

	FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, _Scale);
}

void AMyPlayer::MoveSide(float _Scale)
{
	if (_Scale == 1.f)
		m_PressDKey = true;
	else if (_Scale == -1.f)
		m_PressAKey = true;
	else
	{
		m_PressDKey = false;
		m_PressAKey = false;
	}
	
	if (m_Moveable == false)
		return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, _Scale);
}

void AMyPlayer::Turn(float _Scale)
{
	if (m_LockOnArm->IsLockOn())
	{
		AddControllerYawInput(_Scale * m_LockOnArm->GetLockOnTurnRate() * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerYawInput(_Scale * 0.5f);
	}
}

void AMyPlayer::LookUp(float _Scale)
{
	if (m_LockOnArm->IsLockOn())
	{
		AddControllerPitchInput(_Scale * m_LockOnArm->GetLockOnLookUpRate() * GetWorld()->GetDeltaSeconds());
	}
	else
	{
		AddControllerPitchInput(_Scale * 0.5f);
	}
}

void AMyPlayer::JumpAction()
{
	if (IsDamage())
		return;

	if (m_Jump == false)
	{
		m_JumpAttack = false;
		m_JumpAttackB = false;

		if (IsPressMoveKey())
		{
			FRotator Rot = GetPressKeyRotation();
			FVector Direction;

			Rot.Roll = 0.f;
			Rot.Pitch = 0.f;
			SetActorRotation(Rot);

			GetCharacterMovement()->Velocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
		}

		Jump();

		GetCharacterMovement()->JumpZVelocity = 1000.f;
		GetMovementComponent()->StopMovementImmediately();

		ChangeState(EPLAYER_STATE::JUMP);

		if (IsPressMoveKey())
			GetCharacterMovement()->Velocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
	}
	else if (m_JumpSecond == false)
	{
		if (IsPressMoveKey())
		{
			FRotator Rot = GetPressKeyRotation();
			FVector Direction;

			Rot.Roll = 0.f;
			Rot.Pitch = 0.f;
			SetActorRotation(Rot);

			GetCharacterMovement()->Velocity = GetActorForwardVector() * GetCharacterMovement()->MaxWalkSpeed;
		}

		Jump();

		ChangeState(EPLAYER_STATE::JUMP_SECOND);

		GetCharacterMovement()->JumpZVelocity = 1300.f;
	}
}

void AMyPlayer::AttackAction()
{
	if (IsDamage())
		return;

	if (m_AttackCancleable == false)
		return;

	if (EPLAYER_STATE::SWORD_JUMP_COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::SWORD_JUMP_COMBO_A_2)
	{
		LookToLockOnTarget();
		ChangeState((EPLAYER_STATE)((int)m_State + 1));
	}
	else if (m_Jump == true || m_State == EPLAYER_STATE::SWORD_UPPER_JUMP)
	{
		if (m_JumpAttack == false)
		{
			m_JumpAttack = true;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			LookToLockOnTarget();
			ChangeState(EPLAYER_STATE::SWORD_JUMP_COMBO_A_1);
		}
	}
	else if (EPLAYER_STATE::SWORD_COMBO_A_1 <= m_State
		&& m_State < EPLAYER_STATE::SWORD_COMBO_A_5)
	{
		LookToLockOnTarget();
		ChangeState((EPLAYER_STATE)((int)m_State + 1));
	}
	else
	{
		LookToLockOnTarget();
		ChangeState(EPLAYER_STATE::SWORD_COMBO_A_1);
	}
}

void AMyPlayer::AttackRAction()
{
	if (IsDamage())
		return;

	if (m_AttackCancleable == false)
		return;

	if (m_Jump == true)
	{
		if (m_JumpAttackB == false || m_State == EPLAYER_STATE::SWORD_UPPER_JUMP)
		{
			m_JumpAttack = false;
			m_JumpAttackB = true;
			LookToLockOnTarget();
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			ChangeState(EPLAYER_STATE::SWORD_JUMP_COMBO_B_1);
		}
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_B_1)
	{
		LookToLockOnTarget();
		ChangeState(EPLAYER_STATE::SWORD_COMBO_B_2);
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_B_2)
	{
		LookToLockOnTarget();
		m_ComboBCount = 0;
		ChangeState(EPLAYER_STATE::SWORD_COMBO_B_3);
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_B_3 || m_State == EPLAYER_STATE::SWORD_COMBO_B_4)
	{
		if (m_ComboBCount < 4)
		{
			LookToLockOnTarget();
			EPLAYER_STATE State = (int)m_ComboBCount % 3 == 0 ? EPLAYER_STATE::SWORD_COMBO_B_4 : EPLAYER_STATE::SWORD_COMBO_B_3;
			ChangeState(State, true);
			m_ComboBCount++;
		}
		else
		{
			LookToLockOnTarget();
			ChangeState(EPLAYER_STATE::SWORD_COMBO_B_5);
		}
	}
	else
	{
		LookToLockOnTarget();
		ChangeState(EPLAYER_STATE::SWORD_COMBO_B_1);
	}
}

void AMyPlayer::EvadeAction()
{
	if (IsDamage())
		return;

	if (m_State == EPLAYER_STATE::EVADE || m_Jump == true)
		return;

	FRotator Rot = GetPressKeyRotation();
	FVector Direction;

	Rot.Roll = 0.f;
	Rot.Pitch = 0.f;
	SetActorRotation(Rot);

	GetMovementComponent()->StopMovementImmediately();

	Direction = GetActorForwardVector();
	FVector MoveVector = Direction.GetSafeNormal() * 1200.f;
	MoveVector.Z = 0.f;

	LaunchCharacter(MoveVector, false, false);

	ChangeState(EPLAYER_STATE::EVADE);
}

void AMyPlayer::LockOnDownAction()
{
	m_PressLockOn = true;
	m_PressLockOnTime = GetWorld()->GetRealTimeSeconds();
}

void AMyPlayer::LockOnUpAction()
{
	if (m_PressLockOn == false)
		return;

	m_PressLockOn = false;
	
	m_LockOnArm->LockOn();
}

void AMyPlayer::SpecialAttackDownAction()
{
	if (IsDamage())
		return;

	if (m_Jump == true)
	{
		if (m_PressSKey == true)
		{
			if (EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_START <= m_State && m_State <= EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_END)
				return;

			if (m_LockOnArm->IsLockOn())
				LookToLockOnTarget();

			ChangeState(EPLAYER_STATE::SWORD_JUMP_DOWNATTACK_START);
		}
	}
	else
	{
		if (EPLAYER_STATE::SWORD_UPPER_START <= m_State && m_State <= EPLAYER_STATE::SWORD_DASHATTACK_FINISH_START)
			return;

		if (m_PressSKey == true)
		{
			m_PressSpecialAttackKey = true;

			if (m_LockOnArm->IsLockOn())
				LookToLockOnTarget();

			ChangeState(EPLAYER_STATE::SWORD_UPPER_START);
		}
		else if (m_PressWKey == true)
		{
			if (m_State == EPLAYER_STATE::SWORD_DASHATTACK_FINISH_END)
				return;

			m_PressSpecialAttackKey = true;

			if (m_LockOnArm->IsLockOn())
				LookToLockOnTarget();

			m_DashAttackTimer = 0.f;
			ChangeState(EPLAYER_STATE::SWORD_DASHATTACK_START);
		}
	}
}

void AMyPlayer::SpecialAttackUpAction()
{
	m_PressSpecialAttackKey = false;
}

void AMyPlayer::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	if (_OtherComp->GetCollisionObjectType() == ECC_GameTraceChannel5)
	{
		HitEffect();

		AProjectile* Projectile = Cast<AProjectile>(_OtherActor);
		m_Info.CurHP -= Projectile->GetDamage();

		if (m_Jump == true)
			return;

		FVector TargetDir = _OtherActor->GetActorLocation() - GetActorLocation();
		FRotator TargetRot = TargetDir.GetSafeNormal().Rotation();
		FRotator MyRot = GetControlRotation();
		MyRot.Pitch = 0.f;
		MyRot.Yaw = TargetRot.Yaw;

		SetActorRotation(MyRot);

		GetMovementComponent()->StopMovementImmediately();

		FVector HitVector = GetActorForwardVector() * -500.f;
		HitVector.Z = 0.f;
		LaunchCharacter(HitVector, false, false);

		ChangeState(EPLAYER_STATE::DAMAGE, true);
	}
}

void AMyPlayer::Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo)
{
	Super::Damage(_Actor, _AttackInfo);

	m_Info.CurHP -= _AttackInfo->Damage;

	FString a = FString::Printf(TEXT("Player = %f %f %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	LOG(Log, *a);

	a = FString::Printf(TEXT("Actor = %f %f %f"), _Actor->GetActorLocation().X, _Actor->GetActorLocation().Y, _Actor->GetActorLocation().Z);
	LOG(Log, *a);

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

	LaunchCharacter(HitVector, false, false);

	if (m_State != EPLAYER_STATE::JUMP_DAMAGE_START && m_State != EPLAYER_STATE::JUMP_DAMAGE_LOOP)
	{
		if (0.f < _AttackInfo->KnockBackPowerZ)
		{
			m_Jump = true;
			ChangeState(EPLAYER_STATE::JUMP_DAMAGE_START);
		}
		else
		{
			ChangeState(EPLAYER_STATE::DAMAGE, true);
		}
	}
}

bool AMyPlayer::IsPressMoveKey()
{
	return m_PressWKey || m_PressSKey || m_PressAKey || m_PressDKey;
}

FRotator AMyPlayer::GetPressKeyRotation()
{
	FRotator Rot = GetControlRotation();

	if (m_PressWKey)
	{
		if (m_PressAKey)
			Rot += FRotator(0.f, -45.f, 0.f);
		else if (m_PressDKey)
			Rot += FRotator(0.f, 45.f, 0.f);
	}
	else if (m_PressSKey)
	{
		if (m_PressAKey)
			Rot += FRotator(0.f, 225.f, 0.f);
		else if (m_PressDKey)
			Rot += FRotator(0.f, 135.f, 0.f);
		else
			Rot += FRotator(0.f, 180.f, 0.f);
	}
	else if (m_PressAKey)
	{
		Rot += FRotator(0.f, -90.f, 0.f);
	}
	else if (m_PressDKey)
	{
		Rot += FRotator(0.f, 90.f, 0.f);
	}

	return Rot;
}