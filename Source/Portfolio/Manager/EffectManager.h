#pragma once

#include "../Etc/global.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectManager.generated.h"

UCLASS()
class PORTFOLIO_API UEffectManager : public UObject
{
	GENERATED_BODY()

private:
	static UEffectManager* m_This;
	static UWorld* m_World;

public:
	static UEffectManager* GetInst(UWorld* _World);
	static void Destroy();

public:
	void CreateEffect(UObject* _Object, FTransform& _trans, ULevel* _Level, float _Scale = 1.f);

private:
	UEffectManager();
	~UEffectManager();
};
