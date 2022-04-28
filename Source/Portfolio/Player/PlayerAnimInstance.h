// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class PORTFOLIO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_Speed;

private:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDT) override;

private:
	UFUNCTION()
	void AnimNotify_Idle_L();

	UFUNCTION()
	void AnimNotify_Idle_R();

	UFUNCTION()
	void AnimNotify_Moveable();

	UFUNCTION()
	void AnimNotify_MoveDisable();

	UFUNCTION()
	void AnimNotify_MoveStart();

	UFUNCTION()
	void AnimNotify_MoveStop();

	UFUNCTION()
	void AnimNotify_Attackable();

	UFUNCTION()
	void AnimNotify_CollisionOn();

	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_RunLoop();

	UFUNCTION()
	void AnimNotify_JumpLoop();

	UFUNCTION()
	void AnimNotify_UpperStartEnd();
};