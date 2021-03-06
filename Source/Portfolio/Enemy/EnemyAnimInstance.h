#pragma once

#include "../Etc/global.h"

#include "../Etc/define.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UCLASS()
class PORTFOLIO_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_State;

public:
	UEnemyAnimInstance();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDT) override;

public:
	UFUNCTION()
	void AnimNotify_Idle();

	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_Fire();

	UFUNCTION()
	void AnimNotify_FlyDownCheck();

	UFUNCTION()
	void AnimNotify_Destroy();

	UFUNCTION()
	void AnimNotify_Dissolve();

	UFUNCTION()
	void AnimNotify_DamageAirEnd();

	UFUNCTION()
	void AnimNotify_DecreaseVelocity();

	UFUNCTION()
	void AnimNotify_MoveStart();

	UFUNCTION()
	void AnimNotify_MoveStop();

	UFUNCTION()
	void AnimNotify_MoveSlowStop();

	UFUNCTION()
	void AnimNotify_LookToPlayer();

	UFUNCTION()
	void AnimNotify_Attack6();

	UFUNCTION()
	void AnimNotify_StartLightning();

	UFUNCTION()
	void AnimNotify_EndLightning();
};