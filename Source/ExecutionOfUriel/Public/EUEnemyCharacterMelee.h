// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "EUEnemyCharacter.h"
#include "EUEnemyCharacterMelee.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUEnemyCharacterMelee : public AEUEnemyCharacter
{
	GENERATED_BODY()
	
public:
	AEUEnemyCharacterMelee();

	// =============================================================================
	// Combat

	virtual void OnAttack() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (ClampMin = 0.0))
	float AttackRangeHalfWidth;
};
