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
	ACharacterBase();

protected:
	TArray<USkeletalMeshComponent*> m_EtcMesh;

protected:
	bool m_HitEffect;
	float m_HitEffectTimer;

public:
	virtual void Damage(const AActor* _Actor, const FAttackInfo* _AttackInfo);
	void SpawnProjectile(TSubclassOf<AProjectile> _Particle, const FVector& _Position, const FRotator& _Rotation, const FVector& _Velocity);

protected:
	void HitEffect();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HitEffectUpdate(float _DeltaTime);
};