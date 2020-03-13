// Fill out your copyright notice in the Description page of Project Settings.


#include "EUEnemyCharacterMelee.h"
#include "EUCharacterStatusComponent.h"

AEUEnemyCharacterMelee::AEUEnemyCharacterMelee()
{

}

void AEUEnemyCharacterMelee::OnAttack()
{
	Super::OnAttack();
	
	FVector Start = GetActorLocation() + AttackRangeHalfWidth * GetActorForwardVector();
	FVector End = GetActorLocation() + (AttackRangeDepth - AttackRangeHalfWidth) * GetActorForwardVector();

	FHitResult HitResult;
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5, FCollisionShape::MakeSphere(AttackRangeHalfWidth)
	);
	if (bResult)
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(CharacterStatus->GetAttack(), DamageEvent, GetController(), this);
	}
}