// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayer.h"

AMyPlayer::AMyPlayer()
	: m_State(EPLAYER_STATE::IDLE)
	, m_Moveable(true)
	, m_AttackMove(false)
	, m_AttackCancleable(true)
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

void AMyPlayer::PlayMontage(EPLAYER_STATE _State)
{
	FName RowName = m_MontageMap.FindRef(_State);

	FPlayerMontageInfo* MontageInfo = m_MontageTable->FindRow<FPlayerMontageInfo>(RowName, TEXT(""));
	if (MontageInfo != nullptr)
	{
		m_AnimInst->Montage_Play(MontageInfo->Montage);
	}
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
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AttackMove();
}

void AMyPlayer::AttackMove()
{
	if (m_AttackMove == false)
		return;

	AddMovementInput(GetActorForwardVector(), 1.f);
}

void AMyPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveFront"), this, &AMyPlayer::MoveFront);
	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &AMyPlayer::MoveSide);

	PlayerInputComponent->BindAxis(TEXT("RotationZ"), this, &AMyPlayer::RotationZ);
	PlayerInputComponent->BindAxis(TEXT("RotationY"), this, &AMyPlayer::RotationY);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayer::JumpAction);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyPlayer::AttackAction);
}

void AMyPlayer::ChangeState(EPLAYER_STATE _NextState)
{
	if (m_State == _NextState)
		return;

	m_State = _NextState;

	if (EPLAYER_STATE::COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::COMBO_A_5)
	{
		m_AttackCancleable = false;
	}

	switch (m_State)
	{
	case EPLAYER_STATE::RUN:
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->bUseSeparateBrakingFriction = true;
		PlayMontage(EPLAYER_STATE::RUN);
		break;
	case EPLAYER_STATE::RUN_END:
		PlayMontage(EPLAYER_STATE::RUN_END);
		break;
	case EPLAYER_STATE::COMBO_A_1:
		PlayMontage(EPLAYER_STATE::COMBO_A_1);
		break;
	case EPLAYER_STATE::COMBO_A_2:
		PlayMontage(EPLAYER_STATE::COMBO_A_2);
		break;
	case EPLAYER_STATE::COMBO_A_3:
		PlayMontage(EPLAYER_STATE::COMBO_A_3);
		break;
	case EPLAYER_STATE::COMBO_A_4:
		PlayMontage(EPLAYER_STATE::COMBO_A_4);
		break;
	case EPLAYER_STATE::COMBO_A_5:
		PlayMontage(EPLAYER_STATE::COMBO_A_5);
		break;
	case EPLAYER_STATE::JUMP:
		break;
	case EPLAYER_STATE::DAMAGE:
		break;
	case EPLAYER_STATE::FALL:
		break;
	case EPLAYER_STATE::DEAD:
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
	if (EPLAYER_STATE::COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::COMBO_A_5)
	{
		if (_Scale != 0.f && m_Moveable == true)
		{
			ChangeState(EPLAYER_STATE::RUN);
		}
	}

	if ((m_State == EPLAYER_STATE::IDLE || m_State == EPLAYER_STATE::RUN_END) && _Scale != 0.f)
	{
		ChangeState(EPLAYER_STATE::RUN);
	}
	else if (m_State == EPLAYER_STATE::RUN && _Scale == 0.f)
	{
		ChangeState(EPLAYER_STATE::RUN_END);
	}

	AddMovementInput(GetActorForwardVector(), _Scale);
}

void AMyPlayer::MoveSide(float _Scale)
{
	AddMovementInput(GetActorRightVector() , _Scale);
}

void AMyPlayer::RotationZ(float _Scale)
{
	AddControllerYawInput(_Scale);
}

void AMyPlayer::RotationY(float _Scale)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();

	FRotator Rot = m_Arm->GetRelativeRotation();
	Rot.Pitch += DeltaTime * _Scale * 180.f;

	if (40.f < Rot.Pitch)
		Rot.Pitch = 40.f;
	else if (Rot.Pitch < -45.f)
		Rot.Pitch = -45.f;

	m_Arm->SetRelativeRotation(Rot);
}

void AMyPlayer::JumpAction()
{

}

void AMyPlayer::AttackAction()
{
	if (EPLAYER_STATE::COMBO_A_1 <= m_State
		&& m_State < EPLAYER_STATE::COMBO_A_5)
	{
		if (m_AttackCancleable == true)
		{
			ChangeState((EPLAYER_STATE)((int)m_State + 1));
		}
	}
	else if (m_State == EPLAYER_STATE::COMBO_A_5)
	{
		if (m_AttackCancleable == true)
		{
			ChangeState(EPLAYER_STATE::COMBO_A_1);
		}
	}
	else
	{
		ChangeState(EPLAYER_STATE::COMBO_A_1);
	}
}