#include "WarningMark.h"

#include <Components/DecalComponent.h>
#include "../Effect/BossLightning.h"
#include "../Player/MyPlayer.h"

AWarningMark::AWarningMark()
	: m_LifeTime(2.f)
	, m_CheckLifeTime(0.f)
	, m_CreateEffect(false)
{
	PrimaryActorTick.bCanEverTick = true;

	m_Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	SetRootComponent(m_Decal);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Material/MT_WarningMark.MT_WarningMark'"));
	if (Material.Succeeded())
		m_Decal->SetMaterial(0, Material.Object);

	SetActorRotation(FRotator(-90.f, 0.f, 0.f));
}

void AWarningMark::BeginPlay()
{
	Super::BeginPlay();

	//m_FrontDecal->DecalSize = FVector::ZeroVector;
	//m_FrontDecalInst = m_FrontDecal->CreateDynamicMaterialInstance();
	//m_FrontDecalInst->SetScalarParameterValue(TEXT("Opacity"), 1.f);
}

void AWarningMark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_CheckLifeTime += DeltaTime;
	if (m_CreateEffect == false && m_LifeTime <= m_CheckLifeTime + 0.1f)
	{
		m_CreateEffect = true;

		FHitResult Result;
		FCollisionQueryParams param;

		FVector Pos = GetActorLocation();
		FVector End = Pos;
		End.Z -= 10000.f;

		GetWorld()->LineTraceSingleByChannel(Result, Pos, End, ECC_GameTraceChannel9, param);

		FActorSpawnParameters SpawnParam = {};

		SpawnParam.OverrideLevel = GetLevel();
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMyPlayer* Player = Cast<AMyPlayer>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());

		FVector TargetDir = Player->GetActorLocation() - GetActorLocation();
		FRotator TargetRot = TargetDir.GetSafeNormal().Rotation();
		FRotator MyRot = FRotator(0.f, TargetRot.Yaw, 0.f);

		GetWorld()->SpawnActor<ABossLightning>(ABossLightning::StaticClass(), Result.Location, MyRot, SpawnParam);
	}

	if (m_LifeTime <= m_CheckLifeTime)
		Destroy();
}