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
	void AnimNotify_Fire();

	UFUNCTION()
	void AnimNotify_FlyDownCheck();

	UFUNCTION()
	void AnimNotify_Destroy();

	UFUNCTION()
	void AnimNotify_Dissolve();
};