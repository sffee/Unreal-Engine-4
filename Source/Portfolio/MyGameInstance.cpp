#include "MyGameInstance.h"

#include "Manager/InventoryManager.h"

UMyGameInstance::UMyGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/BlueprintClass/Player/DataTable/PlayerInfoTable.PlayerInfoTable'"));
	if (PlayerTable.Succeeded())
		m_PlayerInfoTable = PlayerTable.Object;

	ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTable(TEXT("DataTable'/Game/BlueprintClass/Item/ItemDataTable.ItemDataTable'"));
	if (ItemDataTable.Succeeded())
	{
		m_ItemDataTable = ItemDataTable.Object;
		UInventoryManager::GetInst(GetWorld())->SetItemDataTable(m_ItemDataTable);
	}
}