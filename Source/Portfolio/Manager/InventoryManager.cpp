#include "InventoryManager.h"

#include "../PortfolioGameModeBase.h"
#include "../UI/Player/MainHUD.h"

UInventoryManager* UInventoryManager::m_This = nullptr;
UWorld* UInventoryManager::m_World = nullptr;

UInventoryManager::UInventoryManager()
{
}

UInventoryManager::~UInventoryManager()
{
}

UInventoryManager* UInventoryManager::GetInst(UWorld* _World)
{
	if (m_This == nullptr)
	{
		m_This = NewObject<UInventoryManager>();
		m_This->AddToRoot();

		m_World = _World;
	}

	if (m_World != _World)
		m_World = _World;

	return m_This;
}

void UInventoryManager::Destroy()
{
	if (m_This != nullptr)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}

void UInventoryManager::SetItemDataTable(UDataTable* _ItemTable)
{
	m_ItemDataTable = _ItemTable;

	FString Str;
	TArray<FItemDataInfo*> AllRows;
	m_ItemDataTable->GetAllRows<FItemDataInfo>(Str, AllRows);

	TArray<FName> RowNames = m_ItemDataTable->GetRowNames();

	for (int i = 0; i < AllRows.Num(); ++i)
		m_ItemNameMap.Add(AllRows[i]->ID, RowNames[i]);
}

const FItemDataInfo* UInventoryManager::GetItemInfo(EITEM_ID _ID)
{
	if (m_ItemDataTable == nullptr)
	{
		LOG(Error, L"ItemDataTable is Nullptr!!");
		return nullptr;
	}

	FName RowName = m_ItemNameMap.FindRef(_ID);
	FItemDataInfo* ItemInfo = m_ItemDataTable->FindRow<FItemDataInfo>(RowName, TEXT(""));

	return ItemInfo;
}

void UInventoryManager::SetItem(EITEM_ID _ID, int _Count)
{
	const FItemDataInfo* ItemInfo = GetItemInfo(_ID);
	if (ItemInfo == nullptr)
		return;

	FInvenData data;
	data.Info = ItemInfo;
	data.Count = _Count;

	ArrInven[(int)ItemInfo->TYPE].Add(_ID, data);

	if (_ID == EITEM_ID::HP_POTION)
	{
		APortfolioGameModeBase* GameMode = Cast<APortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		MainHUD->SetPotionCount(ArrInven[(int)EITEM_TYPE::CONSUMABLE][EITEM_ID::HP_POTION].Count);
	}
}

void UInventoryManager::AddItem(EITEM_ID _ID, int _Count)
{
	const FItemDataInfo* ItemInfo = GetItemInfo(_ID);
	if (ItemInfo == nullptr || _Count < 1)
		return;

	FInvenData* FindData = ArrInven[(int)ItemInfo->TYPE].Find(_ID);

	if (FindData == nullptr)
	{
		FInvenData data;
		data.Info = ItemInfo;
		data.Count = _Count;

		ArrInven[(int)ItemInfo->TYPE].Add(_ID, data);
	}
	else
	{
		FindData->Count += _Count;
	}

	if (_ID == EITEM_ID::HP_POTION)
	{
		APortfolioGameModeBase* GameMode = Cast<APortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		MainHUD->SetPotionCount(ArrInven[(int)EITEM_TYPE::CONSUMABLE][EITEM_ID::HP_POTION].Count);
	}
}

bool UInventoryManager::UseItem(EITEM_ID _ID)
{
	const FItemDataInfo* ItemInfo = GetItemInfo(_ID);
	if (ItemInfo == nullptr)
		return false;

	FInvenData* FindData = ArrInven[(int)ItemInfo->TYPE].Find(_ID);
	if (FindData == nullptr || FindData->Count == 0)
		return false;

	if (_ID == EITEM_ID::HP_POTION)
	{
		APortfolioGameModeBase* GameMode = Cast<APortfolioGameModeBase>(UGameplayStatics::GetGameMode(m_World));
		UMainHUD* MainHUD = GameMode->GetMainHUD();

		if (MainHUD->UsePotion())
		{
			MainHUD->SetPotionCount(--FindData->Count);
			return true;
		}
	}

	return false;
}