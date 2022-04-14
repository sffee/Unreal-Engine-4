// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "global.h"

#include "define.h"

#include "CoreMinimal.h"
#include "MyStruct.generated.h"

class PORTFOLIO_API MyStruct
{
public:
	MyStruct();
	~MyStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerMontageInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* Montage;
};

USTRUCT(Atomic, BlueprintType)
struct FAttackMoveInfo : public FTableRowBase
{
	GENERATED_BODY()

	FAttackMoveInfo()
		: UseSeparateBrakingFriction(false)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float MaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		bool UseSeparateBrakingFriction;
};