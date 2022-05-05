#include "WarningMark.h"

#include <Components/DecalComponent.h>

AWarningMark::AWarningMark()
	: m_LifeTime(2.f)
{
	PrimaryActorTick.bCanEverTick = true;

	m_BackDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("BackDecal"));
	m_FrontDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("FrontDecal"));

	SetRootComponent(m_BackDecal);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Material/MT_WarningMark.MT_WarningMark'"));
	if (Material.Succeeded())
	{
		m_BackDecal->SetMaterial(0, Material.Object);
		m_FrontDecal->SetMaterial(0, Material.Object);
	}

	SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}

void AWarningMark::BeginPlay()
{
	Super::BeginPlay();

	m_FrontDecal->DecalSize = FVector::ZeroVector;
	m_FrontDecalInst = m_FrontDecal->CreateDynamicMaterialInstance();
	m_FrontDecalInst->SetScalarParameterValue(TEXT("Opacity"), 1.f);
}

void AWarningMark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_CheckLifeTime += DeltaTime;
	m_FrontDecal->DecalSize = m_BackDecal->DecalSize * m_CheckLifeTime / m_LifeTime;

	if (m_LifeTime <= m_CheckLifeTime)
		Destroy();
}