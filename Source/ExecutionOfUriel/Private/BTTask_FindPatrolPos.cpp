// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "EUEnemyAIController.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("Find Patrol Pos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	EUCHECK(ControllingPawn != nullptr, EBTNodeResult::Failed);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	EUCHECK(NavSystem != nullptr, EBTNodeResult::Failed);

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEUEnemyAIController::HomePosKey);
	float Radius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AEUEnemyAIController::PatrolRadiusKey);

	FNavLocation NextPatrolLocation;
	bool bGetRandomPoint = NavSystem->GetRandomPointInNavigableRadius(Origin, Radius, NextPatrolLocation);
	EUCHECK(bGetRandomPoint == true, EBTNodeResult::Failed);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEUEnemyAIController::PatrolPosKey, NextPatrolLocation.Location);
	return EBTNodeResult::Succeeded;
}
