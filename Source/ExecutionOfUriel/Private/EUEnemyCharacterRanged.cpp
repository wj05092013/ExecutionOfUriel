// Fill out your copyright notice in the Description page of Project Settings.


#include "EUEnemyCharacterRanged.h"
#include "EUEnemyAIController.h"
#include "EUProjectile.h"
#include "EUCharacterStatusComponent.h"

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

		FireDirection = Target->GetMesh()->GetCenterOfMass() - GetMesh()->GetSocketLocation(ProjectileSpawningSocketName);
		FireDirection.Normalize();
	}
	else
	{
		Super::Attack();
	}
}

void AEUEnemyCharacterRanged::OnAttack()
{
	Super::OnAttack();

	EUCHECK(ProjectileClass != nullptr);

	FVector SpawnLocation = GetMesh()->GetSocketLocation(ProjectileSpawningSocketName);

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;

	auto Projectile = GetWorld()->SpawnActor<AEUProjectile>(ProjectileClass, SpawnLocation, GetActorRotation(), SpawnParam);
	if (Projectile != nullptr)
	{
		Projectile->Fire(FireDirection, nullptr, CharacterStatus->GetMagic());
	}
}
