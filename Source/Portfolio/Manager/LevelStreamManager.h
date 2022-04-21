// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Etc/global.h"

#include <Engine/AssetManager.h>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelStreamManager.generated.h"

UCLASS(Config=Game)
class PORTFOLIO_API ULevelStreamManager : public UObject
{
	GENERATED_BODY()
	
private:
	static ULevelStreamManager* m_This;
	static UWorld* m_World;

public:
	static ULevelStreamManager* GetInst(UWorld* _World);
	static void Destroy();

private:
	UPROPERTY(Config)
	TArray<FString> m_ParticlePaths;

	TMap<FName, UObject*> m_Assets;
	TSharedPtr<FStreamableHandle> m_Handle;

private:
	ULevelStreamManager();
	~ULevelStreamManager();

public:
	void LoadAssetAsync();
	
public:
	UObject* FindAsset(FName _Name);

private:
	void Convert(TArray<FString>& _PathInfo, TArray<UObject*>& _Assets);
	void OnCompleteParticle();
};