// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "EUPlayerCharacter.h"
#include "EUPhaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUPhaseCharacter : public AEUPlayerCharacter
{
	GENERATED_BODY()

public:
	AEUPhaseCharacter();

	// =============================================================================
	// Combat
protected:
	virtual void OnAttack() override;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* LeftHandFlashEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* RightHandFlashEffect;

private:
	static const FName LeftHandSocketName;
	static const FName RightHandSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float TargetingRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class AEUProjectile> ProjectileClass;
};
