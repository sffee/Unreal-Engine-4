// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortfolioGameModeBase.h"
#include "MyGameInstance.h"

#include "UI/Player/MainHUD.h"
#include "UI/Player/PlayerHPBarWidget.h"

#include "Player/MyPlayer.h"

#include "Manager/LevelStreamManager.h"

APortfolioGameModeBase::APortfolioGameModeBase()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI == nullptr)
		return;

	ConstructorHelpers::FClassFinder<APawn> MainCharacterClass(TEXT("Blueprint'/Game/BlueprintClass/Player/BP_Player.BP_Player_C'"));
	if (MainCharacterClass.Succeeded())
		DefaultPawnClass = MainCharacterClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> MainHUD(TEXT("WidgetBlueprint'/Game/BlueprintClass/UI/WBP_MainHUD.WBP_MainHUD_C'"));
	if (MainHUD.Succeeded())
		m_MainHUDClass = MainHUD.Class;
}

void APortfolioGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
	m_MainHUD->AddToViewport();

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	Cast<AMyPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SetPlayerInfo(*GameInstance->GetPlayerInfo());

	ULevelStreamManager::GetInst(GetWorld())->LoadAssetAsync();
}

void APortfolioGameModeBase::UpdateHPBar(float _Ratio)
{
	UPlayerHPBarWidget* HPBar = m_MainHUD->GetHPBarWidget();

	HPBar->SetHP(_Ratio);
}