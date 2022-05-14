#include "LockOnArmComponent.h"
#include "TargetComponent.h"
#include "../Player/MyPlayer.h"
#include "../EnemyBase.h"
#include "../UI/LockOnWidget.h"

ULockOnArmComponent::ULockOnArmComponent()
	: m_IsStart(false)
{
	SetRelativeLocation(FVector( 0.f, 0.f, 100.f ));
	SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	SocketOffset.Z = 70.f;

	m_LockOnTurnRate = 30.f;
	m_LockOnLookUpRate = 45.f;
	m_LockOffPressTime = 0.2f;

	TargetArmLength = 500.0f;
	bUsePawnControlRotation = true;
	bEnableCameraLag = true;
	bEnableCameraRotationLag = false;
	CameraLagSpeed = 3.f;
	CameraRotationLagSpeed = 2.f;
	CameraLagMaxDistance = 100.f;
	m_FindDistance = 2000.f;

	m_LengthMin = 500.f;
	m_LengthMax = 1000.f;
}

void ULockOnArmComponent::BeginPlay()
{
	Super::BeginPlay();

	m_IsStart = true;
}

void ULockOnArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (m_IsStart == false)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LengthUpdate();

	if (m_Target == nullptr)
		return;

	if (IsValid(m_Target) == false || m_Target->IsDeath())
	{
		UTargetComponent* NearTarget = GetNearTarget();
		if (NearTarget == nullptr)
			LockOff();
		else
			LockOn();
	}
}

void ULockOnArmComponent::LengthUpdate()
{
	AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (Player == nullptr)
		return;

	EPLAYER_STATE State = Player->GetState();

	float LengthMin = m_LengthMin;
	float LengthMax = m_LengthMax;

	float ZoomOutStart = 300.f;
	if (State == EPLAYER_STATE::JUMP_DAMAGE_START || State == EPLAYER_STATE::JUMP_DAMAGE_LOOP)
	{
		ZoomOutStart = 500.f;
		LengthMax = 1000.f;
	}

	FHitResult result;
	FCollisionQueryParams param;
	
	FVector Pos = GetOwner()->GetActorLocation();
	FVector End = Pos;
	End.Z -= 10000.f;

	bool Hit = GetWorld()->LineTraceSingleByChannel(result, Pos, End, ECC_GameTraceChannel9, param);

	if (Hit)
	{
		float Distance = result.Distance;
		Distance -= 90.f + LengthMin;

		float Ratio = FMath::Max(0.f, FMath::Min(Distance / ZoomOutStart, 1.f));

		TargetArmLength = (LengthMax - LengthMin) * Ratio + LengthMin;
	}
}

void ULockOnArmComponent::LockOn()
{
	UTargetComponent* NearTarget = GetNearTarget();

	if (NearTarget != nullptr)
	{
		LockOff();

		m_Target = NearTarget;

		AEnemyBase* Enemy = Cast<AEnemyBase>(m_Target->GetOwner());
		UWidgetComponent* Widget = Cast<UWidgetComponent>(Enemy->GetDefaultSubobjectByName(TEXT("LockOnWidget")));
		ULockOnWidget* LockOnWidget = Cast<ULockOnWidget>(Widget->GetWidget());
		LockOnWidget->SetLockOnVisible(true);
	}
}

void ULockOnArmComponent::LockOff()
{
	if (IsValid(m_Target))
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(m_Target->GetOwner());
		UWidgetComponent* Widget = Cast<UWidgetComponent>(Enemy->GetDefaultSubobjectByName(TEXT("LockOnWidget")));
		ULockOnWidget* LockOnWidget = Cast<ULockOnWidget>(Widget->GetWidget());
		LockOnWidget->SetLockOnVisible(false);

		m_Target = nullptr;
	}
}

TArray<UTargetComponent*> ULockOnArmComponent::SearchTarget()
{
	TArray<UTargetComponent*> Targets;

	TArray<FHitResult> arrHit;

	GetWorld()->SweepMultiByChannel(arrHit, GetComponentLocation(), GetComponentLocation(), FQuat::Identity
		, ECC_GameTraceChannel4
		, FCollisionShape::MakeSphere(m_FindDistance));

	if (arrHit.Num())
	{
		for (size_t i = 0; i < arrHit.Num(); ++i)
		{
			UTargetComponent* Target = Cast<UTargetComponent>(arrHit[i].Component);
			
			if (Target->IsDeath() == false)
				Targets.Add(Target);
		}
	}

	return Targets;
}

UTargetComponent* ULockOnArmComponent::GetNearTarget()
{
	TArray<UTargetComponent*> Targets = SearchTarget();

	float NearTargetDistance = 99999.f;
	UTargetComponent* NearTarget = nullptr;

	for (auto& Target : Targets)
	{
		if (m_Target == nullptr)
		{
			NearTargetDistance = FVector::Distance(GetComponentLocation(), Target->GetComponentLocation());
			NearTarget = Target;
			m_Target = Target;
			continue;
		}
		else if (m_Target == Target)
		{
			continue;
		}

		float Distance = FVector::Distance(GetComponentLocation(), Target->GetComponentLocation());
		if (Distance < NearTargetDistance)
		{
			NearTargetDistance = Distance;
			NearTarget = Target;
		}
	}

	return NearTarget;
}

