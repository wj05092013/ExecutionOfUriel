// Fill out your copyright notice in the Description page of Project Settings.


#include "EUEnemyCharacterRanged.h"
#include "EUEnemyAIController.h"

AEUEnemyCharacterRanged::AEUEnemyCharacterRanged()
{
	AttackDelayTimer = 0.3f;
}

void AEUEnemyCharacterRanged::Attack()
{
	if (!bIsAttacking)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackDelayTimerHandle, FTimerDelegate::CreateLambda(
			[this]()->void {
				Super::Attack();
			}
		), AttackDelayTimer, false);

		auto AIController = Cast<AEUEnemyAIController>(GetController());
		EUCHECK(AIController != nullptr);
		auto Target = AIController->GetTarget();
		EUCHECK(Target != nullptr);

		FireDirection = Target->GetActorLocation() - GetActorLocation();
	}
	else
	{
		Super::Attack();
	}
}

void AEUEnemyCharacterRanged::OnAttack()
{
}
