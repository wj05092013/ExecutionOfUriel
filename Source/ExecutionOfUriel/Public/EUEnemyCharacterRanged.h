// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "EUEnemyCharacter.h"
#include "EUEnemyCharacterRanged.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUEnemyCharacterRanged : public AEUEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AEUEnemyCharacterRanged();

	// =============================================================================
	// Combat

	virtual void Attack() override;
	virtual void OnAttack() override;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	FVector FireDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (AllowPrivateAccess = true))
	float AttackDelayTimer;

	FTimerHandle AttackDelayTimerHandle;
};
