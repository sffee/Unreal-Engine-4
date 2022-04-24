#include "LevelStreamManager.h"

#include <Engine/ObjectLibrary.h>

ULevelStreamManager* ULevelStreamManager::m_This = nullptr;
UWorld* ULevelStreamManager::m_World = nullptr;

ULevelStreamManager::ULevelStreamManager()
{
}

ULevelStreamManager::~ULevelStreamManager()
{
}

ULevelStreamManager* ULevelStreamManager::GetInst(UWorld* _World)
{
    if (m_This == nullptr)
    {
        m_This = NewObject<ULevelStreamManager>();
        m_This->AddToRoot();

        m_World = _World;
    }

    if (m_World != _World)
        m_World = _World;

    return m_This;
}

void ULevelStreamManager::Destroy()
{
    if (m_This != nullptr)
    {
        m_This->ConditionalBeginDestroy();
        m_This = nullptr;
        m_World = nullptr;
    }
}


void ULevelStreamManager::LoadAssetAsync()
{
    TArray<FSoftObjectPath> arrPath;

    for (int i = 0; i < m_ParticlePaths.Num(); ++i)
    {
        arrPath.AddUnique(m_ParticlePaths[i]);
    }

    FStreamableManager& streamManager = UAssetManager::GetStreamableManager();
    m_Handle = streamManager.RequestAsyncLoad(arrPath, FStreamableDelegate::CreateUObject(this, &ULevelStreamManager::OnCompleteParticle));
}

UObject* ULevelStreamManager::FindAsset(FName _Name)
{
    UObject* Asset = m_Assets.FindRef(_Name);

    return Asset;
}

void ULevelStreamManager::Convert(TArray<FString>& _PathInfo, TArray<UObject*>& _Assets)
{
    for (int i = 0; i < _PathInfo.Num(); ++i)
    {
        m_Assets.Add(FName(_PathInfo[i]), _Assets[i]);
    }
}

void ULevelStreamManager::OnCompleteParticle()
{
    TArray<UObject*> arrAssets;
    m_Handle->GetLoadedAssets(arrAssets);

    Convert(m_ParticlePaths, arrAssets);
}

