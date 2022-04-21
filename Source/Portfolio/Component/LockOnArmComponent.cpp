#include "LockOnArmComponent.h"
#include "TargetComponent.h"

ULockOnArmComponent::ULockOnArmComponent()
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
}

void ULockOnArmComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_Target == nullptr)
		return;

	if (IsValid(m_Target) == false)
	{
		UTargetComponent* NearTarget = GetNearTarget();
		if (NearTarget == nullptr)
			LockOff();
		else
			LockOn();

		return;
	}
}

void ULockOnArmComponent::LockOn()
{
	UTargetComponent* NearTarget = GetNearTarget();

	if (NearTarget != nullptr)
	{
		m_Target = NearTarget;
		m_Target->LockOn();
	}
}

void ULockOnArmComponent::LockOff()
{
	m_Target->LockOff();
	m_Target = nullptr;
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
			Targets.Add(Cast<UTargetComponent>(arrHit[i].Component));
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