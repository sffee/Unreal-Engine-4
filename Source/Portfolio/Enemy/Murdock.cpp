#include "Murdock.h"

AMurdock::AMurdock()
{
	
}

void AMurdock::BeginPlay()
{
	Super::BeginPlay();
}

void AMurdock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMurdock::Damage(AActor* _Actor)
{
	Super::Damage(_Actor);

	ChangeState(EENEMY_STATE::DAMAGE);

	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), _Actor->GetActorLocation());;
	SetActorRotation(Rotator);
}