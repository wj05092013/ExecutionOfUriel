// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "EUCharacter.h"
#include "EUEnemyCharacter.generated.h"

UCLASS()
class EXECUTIONOFURIEL_API AEUEnemyCharacter : public AEUCharacter
{
	GENERATED_BODY()

public:
	AEUEnemyCharacter();

	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AEUEnemyAIController* EnemyAIController;

	// =============================================================================
	// Status
public:
	virtual void OnDead() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle;

	// =============================================================================
	// Animation
public:
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// =============================================================================
	// Combat

	virtual void Attack() override;

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;
};
