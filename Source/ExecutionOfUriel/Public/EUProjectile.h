// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "GameFramework/Actor.h"
#include "EUProjectile.generated.h"

UCLASS()
class EXECUTIONOFURIEL_API AEUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AEUProjectile();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Fire(const FVector& ShootDirection, TWeakObjectPtr<USceneComponent> TargetComp, float AttackDamage);

private:
	UFUNCTION()
	void OnEnemyCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSeep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplosionEffectFinished(UParticleSystemComponent* ParticleSystem);

	void StartDestroy();

public:
	UPROPERTY(VisibleAnywhere, Category = Collision)
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* FlyingEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* ExplosionEffect;

private:
	FVector StartPos;

	float Damage;

	bool bSetDestroy;
};
