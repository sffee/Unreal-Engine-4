// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyGameInstance.h"
#include "PortfolioGameModeBase.h"

APortfolioGameModeBase::APortfolioGameModeBase()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI == nullptr)
		return;

	ConstructorHelpers::FClassFinder<APawn> MainCharacterClass(TEXT("Blueprint'/Game/BlueprintClass/Player/BP_Player.BP_Player_C'"));
	if (MainCharacterClass.Succeeded())
		DefaultPawnClass = MainCharacterClass.Class;
}