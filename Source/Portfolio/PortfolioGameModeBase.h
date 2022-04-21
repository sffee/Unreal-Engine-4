// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Etc/global.h"

#include <blueprint/UserWidget.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PortfolioGameModeBase.generated.h"

UCLASS()
class PORTFOLIO_API APortfolioGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	APortfolioGameModeBase();

private:
	TSubclassOf<UUserWidget> m_MainHUDClass;
	class UMainHUD* m_MainHUD;

public:
	void UpdateHPBar(float _Ratio);

public:
	virtual void BeginPlay() override;
};
