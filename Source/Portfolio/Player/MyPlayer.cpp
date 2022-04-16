// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"

AMyPlayer::AMyPlayer()
	: m_State(EPLAYER_STATE::SWORD_IDLE_L)
	, m_Moveable(true)
	, m_Attacking(false)
	, m_AttackMove(false)
	, m_AttackCancleable(true)
	, m_ComboBCount(0)
	, m_PressMoveSide(false)
	, m_PressMoveFront(false)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_Arm->SetupAttachment(GetMesh());
	m_Cam->SetupAttachment(m_Arm);

	m_Arm->TargetArmLength = 600.f;

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
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	FString Str;
	TArray<FPlayerMontageInfo*> AllRows;
	m_MontageTable->GetAllRows<FPlayerMontageInfo>(Str, AllRows);

	TArray<FName> AllRowNames = m_MontageTable->GetRowNames();;

	for (int i = 0; i < AllRows.Num(); i++)
		m_MontageMap.Add(AllRows[i]->State, AllRowNames[i]);

	PlayMontage(EPLAYER_STATE::SWORD_IDLE_L);

	APlayerCameraManager* CameraManager = Cast<APlayerController>(Controller)->PlayerCameraManager;
	CameraManager->ViewPitchMax = 10.f;
	CameraManager->ViewPitchMin = -30.f;
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckRunAnimation();
	AttackMove();
}

void AMyPlayer::PlayMontage(EPLAYER_STATE _State)
{
	FName RowName = m_MontageMap.FindRef(_State);

	FPlayerMontageInfo* MontageInfo = m_MontageTable->FindRow<FPlayerMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
		m_AnimInst->Montage_Play(MontageInfo->Montage);
}

void AMyPlayer::AttackMove()
{
	if (m_AttackMove == false)
		return;

	AddMovementInput(GetActorForwardVector(), 1.f);
}

void AMyPlayer::CheckRunAnimation()
{
	if (m_Attacking && (m_PressMoveSide || m_PressMoveFront) && m_Moveable == true)
	{
		ChangeState(EPLAYER_STATE::SWORD_RUN);
		return;
	}

	if ((m_State == EPLAYER_STATE::SWORD_IDLE_L || m_State == EPLAYER_STATE::SWORD_IDLE_R || m_State == EPLAYER_STATE::SWORD_RUN_END)
		&& (m_PressMoveSide || m_PressMoveFront) != 0.f)
	{
		ChangeState(EPLAYER_STATE::SWORD_RUN);
		return;
	}

	if (m_State == EPLAYER_STATE::SWORD_RUN
		&& (!m_PressMoveSide && !m_PressMoveFront))
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

	PlayerInputComponent->BindAxis(TEXT("RotationZ"), this, &AMyPlayer::RotationZ);
	PlayerInputComponent->BindAxis(TEXT("RotationY"), this, &AMyPlayer::RotationY);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayer::JumpAction);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayer::AttackAction);
	PlayerInputComponent->BindAction(TEXT("AttackR"), EInputEvent::IE_Pressed, this, &AMyPlayer::AttackRAction);
}

void AMyPlayer::ChangeState(EPLAYER_STATE _NextState, bool _Ignore)
{
	if (_Ignore == false && m_State == _NextState)
		return;

	m_State = _NextState;

	if (EPLAYER_STATE::SWORD_COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::SWORD_COMBO_B_5)
	{
		m_Attacking = true;
		m_Moveable = false;
		m_AttackCancleable = false;
	}
	else
	{
		m_Attacking = false;
	}

	PlayMontage(m_State);

	switch (m_State)
	{
	case EPLAYER_STATE::SWORD_IDLE_L:
		break;
	case EPLAYER_STATE::SWORD_IDLE_R:
		break;
	case EPLAYER_STATE::SWORD_RUN:
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->bUseSeparateBrakingFriction = true;
		break;
	case EPLAYER_STATE::SWORD_RUN_END:
		break;
	case EPLAYER_STATE::SWORD_COMBO_A_1:
		break;
	case EPLAYER_STATE::SWORD_COMBO_A_2:
		break;
	case EPLAYER_STATE::SWORD_COMBO_A_3:
		break;
	case EPLAYER_STATE::SWORD_COMBO_A_4:
		break;
	case EPLAYER_STATE::SWORD_COMBO_A_5:
		break;
	case EPLAYER_STATE::SWORD_JUMP:
		break;
	case EPLAYER_STATE::SWORD_DAMAGE:
		break;
	case EPLAYER_STATE::SWORD_FALL:
		break;
	case EPLAYER_STATE::SWORD_DEAD:
		break;
	default:
		break;
	}
}

void AMyPlayer::AttackMoveSpeedSetting(EPLAYER_STATE _State)
{
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	FName RowName = m_MontageMap.FindRef(_State);

	FPlayerMontageInfo* MontageInfo = m_MontageTable->FindRow<FPlayerMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
	{
		FName Section = m_AnimInst->Montage_GetCurrentSection(MontageInfo->Montage);
		FAttackMoveInfo* AttackMoveInfo = m_AttackMoveTable->FindRow<FAttackMoveInfo>(Section, TEXT(""));
		if (AttackMoveInfo != nullptr)
		{
			GetCharacterMovement()->MaxWalkSpeed = AttackMoveInfo->MaxWalkSpeed;
			GetCharacterMovement()->MaxAcceleration = AttackMoveInfo->MaxAcceleration;
			GetCharacterMovement()->bUseSeparateBrakingFriction = AttackMoveInfo->UseSeparateBrakingFriction;
		}
	}
}

void AMyPlayer::MoveFront(float _Scale)
{
	if (_Scale != 0.f)
		m_PressMoveFront = true;
	else
		m_PressMoveFront = false;

	if (m_Moveable == false)
		return;

	FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, _Scale);
}

void AMyPlayer::MoveSide(float _Scale)
{
	if (_Scale != 0.f)
		m_PressMoveSide = true;
	else
		m_PressMoveSide = false;

	if (m_Moveable == false)
		return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, _Scale);
}

void AMyPlayer::RotationZ(float _Scale)
{
	AddControllerYawInput(_Scale);
}

void AMyPlayer::RotationY(float _Scale)
{
	AddControllerPitchInput(_Scale);
}

void AMyPlayer::JumpAction()
{ 
}

void AMyPlayer::AttackAction()
{
	if (EPLAYER_STATE::SWORD_COMBO_A_1 <= m_State
		&& m_State < EPLAYER_STATE::SWORD_COMBO_A_5)
	{
		if (m_AttackCancleable == true)
		{
			ChangeState((EPLAYER_STATE)((int)m_State + 1));
		}
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_A_5)
	{
		if (m_AttackCancleable == true)
		{
			ChangeState(EPLAYER_STATE::SWORD_COMBO_A_1);
		}
	}
	else
	{
		ChangeState(EPLAYER_STATE::SWORD_COMBO_A_1);
	}
}

void AMyPlayer::AttackRAction()
{
	if (m_State == EPLAYER_STATE::SWORD_COMBO_B_1)
	{
		if (m_AttackCancleable == true)
			ChangeState(EPLAYER_STATE::SWORD_COMBO_B_2);
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_B_2)
	{
		if (m_AttackCancleable == true)
		{
			m_ComboBCount = 0;
			ChangeState(EPLAYER_STATE::SWORD_COMBO_B_3);
		}
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_B_3 || m_State == EPLAYER_STATE::SWORD_COMBO_B_4)
	{
		if (m_ComboBCount < 6)
		{
			if (m_AttackCancleable == true)
			{
				EPLAYER_STATE State = (int)m_ComboBCount % 3 == 0 ? EPLAYER_STATE::SWORD_COMBO_B_4 : EPLAYER_STATE::SWORD_COMBO_B_3;
				ChangeState(State, true);
				m_ComboBCount++;
			}
		}
		else
		{
			if (m_AttackCancleable == true)
				ChangeState(EPLAYER_STATE::SWORD_COMBO_B_5);
		}
	}
	else if (m_State == EPLAYER_STATE::SWORD_COMBO_B_5)
	{
		if (m_AttackCancleable == true)
			ChangeState(EPLAYER_STATE::SWORD_COMBO_B_1);
	}
	else
	{
		ChangeState(EPLAYER_STATE::SWORD_COMBO_B_1);
	}
}