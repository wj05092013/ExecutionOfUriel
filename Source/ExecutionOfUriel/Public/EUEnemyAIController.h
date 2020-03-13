// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "AIController.h"
#include "EUEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEUEnemyAIController();

	void RunAI();
	void StopAI();
	void HitBy(class AEUCharacter* Attacker);

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName PatrolRadiusKey;
	static const FName TargetKey;

	static const float PatrolRadius;
	static const float DetectRadius;
	static const float ChaseRadius;

private:
	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTAsset;
};
