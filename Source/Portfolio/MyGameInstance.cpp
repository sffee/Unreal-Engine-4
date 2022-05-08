#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/BlueprintClass/Player/DataTable/PlayerInfoTable.PlayerInfoTable'"));
	if (PlayerTable.Succeeded())
		m_PlayerInfoTable = PlayerTable.Object;
}