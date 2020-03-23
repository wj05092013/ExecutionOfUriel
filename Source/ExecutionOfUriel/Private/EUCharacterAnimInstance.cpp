// Fill out your copyright notice in the Description page of Project Settings.


#include "EUCharacterAnimInstance.h"
#include "EUCharacter.h"

UEUCharacterAnimInstance::UEUCharacterAnimInstance()
{
	bIsDead = false;
	bIsInAir = false;
}

void UEUCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto EUCharacter = Cast<AEUCharacter>(TryGetPawnOwner());
	if (!IsValid(EUCharacter) || bIsDead)
		return;

	bIsInAir = EUCharacter->GetMovementComponent()->IsFalling();

	Velocity = EUCharacter->GetVelocity();

	AimRotation = (EUCharacter->GetBaseAimRotation() - EUCharacter->GetActorRotation()).GetNormalized();
}

void UEUCharacterAnimInstance::PlayAttackMontage()
{
	EUCHECK(AttackMontage != nullptr);
	EUCHECK(!bIsDead);
	EULOG_S(Warning);
	Montage_Play(AttackMontage);
}

void UEUCharacterAnimInstance::JumpToAttackMontageSection(int32 SectionIndex)
{
	EUCHECK(Montage_IsPlaying(AttackMontage));
	EUCHECK(!bIsDead);
	EUCHECK(FMath::IsWithin(SectionIndex, 0, MontageSectionNum()));

	Montage_JumpToSection(AttackMontageSectionNames[SectionIndex], AttackMontage);
}

int32 UEUCharacterAnimInstance::MontageSectionNum() const
{
	return AttackMontageSectionNames.Num();
}

void UEUCharacterAnimInstance::AnimNotify_Attack()
{
	OnAttack.Broadcast();
}
