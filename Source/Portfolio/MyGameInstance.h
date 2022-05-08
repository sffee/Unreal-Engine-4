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

public:
	FPlayerInfo* GetPlayerInfo()
	{
		return m_PlayerInfoTable->FindRow<FPlayerInfo>(FName(TEXT("Player")), TEXT(""));
	}

public:
	UMyGameInstance();
};