// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Etc/global.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class PORTFOLIO_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_Stateasd;

public:
	ACharacterBase();

public:
	virtual void Damage(AActor* _Actor) {};

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};