#pragma once

#include "Etc/global.h"

#include "Manager/LevelStreamManager.h"
#include "Projectile/Projectile.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class PORTFOLIO_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, meta = (AllowPrivateAccess = "true"))
	EENEMY_STATE m_State;

public:
	ACharacterBase();

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo) {};
	void SpawnProjectile(TSubclassOf<AProjectile> _Particle, const FVector& _Position, const FRotator& _Rotation, const FVector& _Velocity);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};