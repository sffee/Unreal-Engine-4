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

	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_Run.AM_Run'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_Run_End.AM_Run_End'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_ComboA_1.AM_ComboA_1'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_ComboA_2.AM_ComboA_2'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_ComboA_3.AM_ComboA_3'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_ComboA_4.AM_ComboA_4'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_ComboA_5.AM_ComboA_5'"));
	AddAnimMontage(TEXT("AnimMontage'/Game/BlueprintClass/Player/Montage/AM_ComboA_6.AM_ComboA_6'"));
}

void AMyPlayer::AddAnimMontage(const FString _FileName)
{
	ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(*_FileName);
	if (Montage.Succeeded())
		m_arrMontage.Add(Montage.Object);
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	m_AnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
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

	if (EPLAYER_STATE::COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::COMBO_A_6)
	{
		m_AttackCancleable = false;
		AttackMoveSpeedSetting(_NextState);
	}

	switch (m_State)
	{
	case EPLAYER_STATE::RUN:
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		GetCharacterMovement()->MaxAcceleration = 2048.f;
		GetCharacterMovement()->bUseSeparateBrakingFriction = true;
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::RUN]);
		break;
	case EPLAYER_STATE::RUN_END:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::RUN_END]);
		break;
	case EPLAYER_STATE::COMBO_A_1:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::COMBO_A_1]);
		break;
	case EPLAYER_STATE::COMBO_A_2:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::COMBO_A_2]);
		break;
	case EPLAYER_STATE::COMBO_A_3:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::COMBO_A_3]);
		break;
	case EPLAYER_STATE::COMBO_A_4:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::COMBO_A_4]);
		break;
	case EPLAYER_STATE::COMBO_A_5:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::COMBO_A_5]);
		break;
	case EPLAYER_STATE::COMBO_A_6:
		m_AnimInst->Montage_Play(m_arrMontage[(int)EMONTAGE::COMBO_A_6]);
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

	switch (m_State)
	{
	case EPLAYER_STATE::COMBO_A_1:
		GetCharacterMovement()->MaxWalkSpeed = 2000.f;
		GetCharacterMovement()->MaxAcceleration = 1500.f;
		GetCharacterMovement()->bUseSeparateBrakingFriction = false;
		break;
	case EPLAYER_STATE::COMBO_A_2:
		GetCharacterMovement()->MaxWalkSpeed = 2000.f;
		GetCharacterMovement()->MaxAcceleration = 2000.f;
		break;
	case EPLAYER_STATE::COMBO_A_3:
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		GetCharacterMovement()->MaxAcceleration = 2200.f;
		break;
	case EPLAYER_STATE::COMBO_A_4:
		GetCharacterMovement()->MaxWalkSpeed = 1500.f;
		GetCharacterMovement()->MaxAcceleration = 2500.f;
		break;
	case EPLAYER_STATE::COMBO_A_5:
		GetCharacterMovement()->MaxWalkSpeed = 2000.f;
		GetCharacterMovement()->MaxAcceleration = 2500.f;
		break;
	case EPLAYER_STATE::COMBO_A_6:
		GetCharacterMovement()->MaxWalkSpeed = 2000.f;
		GetCharacterMovement()->MaxAcceleration = 4000.f;
		break;
	default:
		break;
	}
}

void AMyPlayer::MoveFront(float _Scale)
{
	if (EPLAYER_STATE::COMBO_A_1 <= m_State && m_State <= EPLAYER_STATE::COMBO_A_6)
	{
		if (_Scale != 0.f && m_Moveable == true)
		{
			ChangeState(EPLAYER_STATE::RUN);
		}
	}

	if (m_State == EPLAYER_STATE::IDLE && _Scale != 0.f)
	{
		ChangeState(EPLAYER_STATE::RUN);
	}
	else if (m_State == EPLAYER_STATE::RUN && _Scale == 0.f)
	{
		ChangeState(EPLAYER_STATE::RUN_END);
	}

	FString Str;
	Str = FString::SanitizeFloat(_Scale);
	LOG(Warning, *Str)

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
		&& m_State < EPLAYER_STATE::COMBO_A_6)
	{
		if (m_AttackCancleable == true)
		{
			ChangeState((EPLAYER_STATE)((int)m_State + 1));
		}
	}
	else if (m_State == EPLAYER_STATE::COMBO_A_6)
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