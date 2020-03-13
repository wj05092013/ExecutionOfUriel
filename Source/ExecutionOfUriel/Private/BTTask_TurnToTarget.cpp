// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "EUCharacter.h"
#include "EUEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
	TurnSpeed = 2.0f;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AEUCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	EUCHECK(ControllingPawn != nullptr, EBTNodeResult::Failed);

	auto Target = Cast<AEUCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEUEnemyAIController::TargetKey));
	EUCHECK(Target != nullptr, EBTNodeResult::Failed);

	FVector ToTarget = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	ToTarget.Z = 0.0f;

	FRotator ToTargetRot = FRotationMatrix::MakeFromX(ToTarget).Rotator();
	FRotator NewRot = FMath::RInterpTo(ControllingPawn->GetActorRotation(), ToTargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed);
	ControllingPawn->SetActorRotation(NewRot);

	return EBTNodeResult::Succeeded;
}