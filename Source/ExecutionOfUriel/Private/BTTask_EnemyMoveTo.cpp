// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyMoveTo.h"
#include "EUEnemyAIController.h"
#include "EUCharacter.h"

UBTTask_EnemyMoveTo::UBTTask_EnemyMoveTo()
{
	NodeName = TEXT("Enemy Move To");
}

EBTNodeResult::Type UBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Result != EBTNodeResult::Succeeded)
		return Result;

	auto ControllingPawn = Cast<AEUCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	EUCHECK(ControllingPawn != nullptr, EBTNodeResult::Failed);

	AcceptableRadius = ControllingPawn->GetAttackRange() - 70;

	return EBTNodeResult::Succeeded;
}