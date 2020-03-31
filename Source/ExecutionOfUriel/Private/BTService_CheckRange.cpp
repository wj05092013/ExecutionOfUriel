// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckRange.h"
#include "EUEnemyAIController.h"
#include "EUCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckRange::UBTService_CheckRange()
{
	Interval = 0.5f;
}

void UBTService_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<AEUCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	EUCHECK(ControllingPawn != nullptr);

	auto TargetPawn = Cast<AEUCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEUEnemyAIController::TargetKey));
	EUCHECK(TargetPawn != nullptr);

	FVector Diff = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
	if (Diff.Size() < ControllingPawn->GetAttackRangeDepth() - 10)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEUEnemyAIController::IsInAttackRangeKey, true);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AEUEnemyAIController::IsInAttackRangeKey, false);
	}
}