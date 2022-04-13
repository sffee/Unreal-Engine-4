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

private:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _fDT) override;

private:
	UFUNCTION()
	void AnimNotify_ActionEnd();

	UFUNCTION()
	void AnimNotify_Run_End();

	UFUNCTION()
	void AnimNotify_Moveable();

	UFUNCTION()
	void AnimNotify_MoveStart();

	UFUNCTION()
	void AnimNotify_MoveStop();

	UFUNCTION()
	void AnimNotify_Cancleable();
};