#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "LockOnArmComponent.generated.h"

class UTargetComponent;
UCLASS(meta = (BlueprintSpawnableComponent))
class PORTFOLIO_API ULockOnArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On", meta = (AllowPrivateAccess = "true"))
	float m_FindDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On", meta = (AllowPrivateAccess = "true"))
	float m_LockOnTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On", meta = (AllowPrivateAccess = "true"))
	float m_LockOnLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On", meta = (AllowPrivateAccess = "true"))
	float m_LockOffPressTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTargetComponent* m_Target;

public:
	ULockOnArmComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool IsLockOn()
	{
		return m_Target != nullptr;
	}

	float GetFindDistance()
	{
		return m_FindDistance;
	}

	float GetLockOnTurnRate()
	{
		return m_LockOnTurnRate;
	}

	float GetLockOnLookUpRate()
	{
		return m_LockOnLookUpRate;
	}

	float GetLockOffPressTime()
	{
		return m_LockOffPressTime;
	}

	UTargetComponent* GetLockOnTarget()
	{
		return m_Target;
	}

public:
	void LockOn();
	void LockOff();

private:
	TArray<UTargetComponent*> SearchTarget();
	UTargetComponent* GetNearTarget();
};