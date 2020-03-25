// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "EUEnemyAIController.h"
#include "EUPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Blackboard = OwnerComp.GetBlackboardComponent();

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	EUCHECK(ControllingPawn != nullptr);
	
	auto TargetCharacter = Cast<AEUPlayerCharacter>(Blackboard->GetValueAsObject(AEUEnemyAIController::TargetKey));

	// It already has a target.
	if (TargetCharacter != nullptr)
	{
		// Target was dead or is out of range. Stop chasing.
		if (TargetCharacter->GetCharacterObjectState() == ECharacterObjectState::Dead
			|| ControllingPawn->GetDistanceTo(TargetCharacter) > AEUEnemyAIController::ChaseRadius)
			Blackboard->SetValueAsObject(AEUEnemyAIController::TargetKey, nullptr);
	}
	else
	{
		FVector Pos = ControllingPawn->GetActorLocation();

		TArray<FOverlapResult> OverlapResults;
		bool bResult = ControllingPawn->GetWorld()->OverlapMultiByChannel(
			OverlapResults, Pos, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel5,
			FCollisionShape::MakeSphere(AEUEnemyAIController::DetectRadius));

		// Get nearest character out of detected characters.
		//

		if (bResult)
		{
			float MinDist = MAX_FLT;

			for (auto OverlapResult : OverlapResults)
			{
				AEUPlayerCharacter* EUCharacter = Cast<AEUPlayerCharacter>(OverlapResult.GetActor());

				if (EUCharacter != nullptr && EUCharacter->GetCharacterObjectState() == ECharacterObjectState::Alive)
				{
					float Dist = FVector::Dist(ControllingPawn->GetActorLocation(), EUCharacter->GetActorLocation());
					if (Dist < MinDist)
					{
						TargetCharacter = EUCharacter;
						MinDist = Dist;
					}
				}
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEUEnemyAIController::TargetKey, TargetCharacter);
	}
}