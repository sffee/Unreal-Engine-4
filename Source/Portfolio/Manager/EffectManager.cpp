// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectManager.h"
#include "../Effect/MyEffect.h"

UEffectManager* UEffectManager::m_This = nullptr;
UWorld* UEffectManager::m_World = nullptr;

UEffectManager::UEffectManager()
{
}

UEffectManager::~UEffectManager()
{
}

UEffectManager* UEffectManager::GetInst(UWorld* _World)
{
	if (m_This == nullptr)
	{
		m_This = NewObject<UEffectManager>();
		m_This->AddToRoot();

		m_World = _World;
	}

	if (m_World != _World)
		m_World = _World;

	return m_This;
}

void UEffectManager::Destroy()
{
	if (m_This != nullptr)
	{
		m_This->ConditionalBeginDestroy();
		m_This = nullptr;
		m_World = nullptr;
	}
}


void UEffectManager::CreateEffect(UObject* _Object, FTransform& _trans, ULevel* _Level, float _Scale)
{
	FActorSpawnParameters SpawnParam = {};

	SpawnParam.OverrideLevel = _Level;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	AMyEffect* pEffect = m_World->SpawnActor<AMyEffect>(AMyEffect::StaticClass(), _trans, SpawnParam);
	pEffect->SetObject(_Object, _Scale);
	pEffect->FinishSpawning(pEffect->GetTransform());
}