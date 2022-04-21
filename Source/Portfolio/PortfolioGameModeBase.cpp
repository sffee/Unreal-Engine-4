// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortfolioGameModeBase.h"
#include "MyGameInstance.h"

#include "UI/Player/MainHUD.h"
#include "UI/Player/PlayerHPBarWidget.h"

APortfolioGameModeBase::APortfolioGameModeBase()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI == nullptr)
		return;

	ConstructorHelpers::FClassFinder<APawn> MainCharacterClass(TEXT("Blueprint'/Game/BlueprintClass/Player/BP_Player.BP_Player_C'"));
	if (MainCharacterClass.Succeeded())
		DefaultPawnClass = MainCharacterClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> MainHUD(TEXT("WidgetBlueprint'/Game/BlueprintClass/UI/MainHUD.MainHUD_C'"));
	if (MainHUD.Succeeded())
		m_MainHUDClass = MainHUD.Class;
}

void APortfolioGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	m_MainHUD = Cast<UMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
	m_MainHUD->AddToViewport();
}

void APortfolioGameModeBase::UpdateHPBar(float _Ratio)
{
	UPlayerHPBarWidget* HPBar = m_MainHUD->GetHPBarWidget();

	HPBar->SetHP(_Ratio);
}