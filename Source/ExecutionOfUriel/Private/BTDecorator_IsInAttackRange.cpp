// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "EUEnemyAIController.h"
#include "EUCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("Can Attack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEUCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	EUCHECK(ControllingPawn != nullptr, false);

	auto Target = Cast<AEUCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEUEnemyAIController::TargetKey));
	EUCHECK(Target != nullptr, false);

	bResult = (ControllingPawn->GetDistanceTo(Target) <= ControllingPawn->GetAttackRange());
	return bResult;
}