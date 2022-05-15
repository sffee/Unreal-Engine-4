#pragma once

#include "../Etc/global.h"
#include "../Etc/define.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager.generated.h"

USTRUCT()
struct FInvenData
{
	GENERATED_BODY()
	const FItemDataInfo* Info;
	int Count;
};

UCLASS()
class PORTFOLIO_API UInventoryManager : public UObject
{
	GENERATED_BODY()
	
private:
	static UInventoryManager* m_This;
	static UWorld* m_World;

public:
	static UInventoryManager* GetInst(UWorld* _world);
	static void Destroy();

private:
	TMap<EITEM_ID, FName> m_ItemNameMap;
	UDataTable* m_ItemDataTable;

	TMap<EITEM_ID, FInvenData> ArrInven[(int)EITEM_TYPE::END];

public:
	void SetItemDataTable(UDataTable* _ItemTable);
	const FItemDataInfo* GetItemInfo(EITEM_ID _ID);
	void SetItem(EITEM_ID _ID, int _Count);
	void AddItem(EITEM_ID _ID, int _Count = 1);
	bool UseItem(EITEM_ID _ID);

private:
	UInventoryManager();
	~UInventoryManager();
};
