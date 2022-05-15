#pragma once

#include "Etc/global.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class PORTFOLIO_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UDataTable* m_PlayerInfoTable;
	UDataTable* m_ItemDataTable;

	bool m_GameStart;

public:
	FPlayerInfo* GetPlayerInfo()
	{
		return m_PlayerInfoTable->FindRow<FPlayerInfo>(FName(TEXT("Player")), TEXT(""));
	}

	bool IsGameStart()
	{
		return m_GameStart;
	}

	void GameStart()
	{
		m_GameStart = true;
	}

public:
	UMyGameInstance();
};