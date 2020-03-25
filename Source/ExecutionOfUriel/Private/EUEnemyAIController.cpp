// Fill out your copyright notice in the Description page of Project Settings.


#include "EUEnemyAIController.h"
#include "EUCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AEUEnemyAIController::HomePosKey(TEXT("HomePos"));
const FName AEUEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEUEnemyAIController::PatrolRadiusKey(TEXT("PatrolRadius"));
const FName AEUEnemyAIController::TargetKey(TEXT("Target"));

const float AEUEnemyAIController::PatrolRadius = 500.0f;
const float AEUEnemyAIController::DetectRadius = 1000.0f;
const float AEUEnemyAIController::ChaseRadius = 1500.0f;

AEUEnemyAIController::AEUEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB_Enemy(TEXT("/Game/ExecutionOfUriel/AI/BB_EUEnemyCharacter.BB_EUEnemyCharacter"));
	EUCHECK(BB_Enemy.Succeeded());
	BBAsset = BB_Enemy.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_Enemy(TEXT("/Game/ExecutionOfUriel/AI/BT_EUEnemyCharacter.BT_EUEnemyCharacter"));
	EUCHECK(BT_Enemy.Succeeded());
	BTAsset = BT_Enemy.Object;
}

void AEUEnemyAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		Blackboard->SetValueAsFloat(PatrolRadiusKey, PatrolRadius);

		if (!RunBehaviorTree(BTAsset))
		{
			EULOG(Error, TEXT("Couldn't run the behavior tree."));
		}
	}
}

void AEUEnemyAIController::StopAI()
{
	auto BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent != nullptr)
	{
		BTComponent->StopTree(EBTStopMode::Safe);
	}
}

void AEUEnemyAIController::HitBy(AEUCharacter* Attacker)
{
	EUCHECK(Attacker != nullptr);
	Blackboard->SetValueAsObject(TargetKey, Attacker);
}

AEUCharacter* AEUEnemyAIController::GetTarget()
{
	auto Target = Cast<AEUCharacter>(Blackboard->GetValueAsObject(TargetKey));
	EUCHECK(Target != nullptr, nullptr);

	return Target;
}
