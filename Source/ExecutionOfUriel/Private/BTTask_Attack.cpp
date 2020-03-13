// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "EUEnemyAIController.h"
#include "EUCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	bIsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEUCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	EUCHECK(ControllingPawn != nullptr, EBTNodeResult::Failed);

	ControllingPawn->Attack();
	bIsAttacking = true;

	ControllingPawn->OnAttackEnd.AddLambda([this]()->void {
		bIsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!bIsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
