// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffect.h"

// Sets default values
AMyEffect::AMyEffect()
{
	m_Particle = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComponent");
	
	SetRootComponent(m_Particle);
}

// Called when the game starts or when spawned
void AMyEffect::BeginPlay()
{
	Super::BeginPlay();
	
	UParticleSystem* ParticleAsset = Cast<UParticleSystem>(m_Object);
	if (ParticleAsset != nullptr)
	{
		m_Particle->SetTemplate(ParticleAsset);
		m_Particle->OnSystemFinished.AddDynamic(this, &AMyEffect::OnFinish);
	}
}

// Called every frame
void AMyEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyEffect::SetObject(UObject* _Object, float _Scale)
{
	m_Object = _Object;
	SetActorScale3D(FVector(_Scale));
}

void AMyEffect::OnFinish(UParticleSystemComponent* _Particle)
{
	Destroy();
}