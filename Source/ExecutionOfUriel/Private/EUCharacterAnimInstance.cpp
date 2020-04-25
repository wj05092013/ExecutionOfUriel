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

	EUCharacter = Cast<AEUCharacter>(TryGetPawnOwner());
	if (!IsValid(EUCharacter) || bIsDead)
		return;

	bIsInAir = EUCharacter->GetMovementComponent()->IsFalling();

	Velocity = EUCharacter->GetVelocity();

	AimRotation = (EUCharacter->GetBaseAimRotation() - EUCharacter->GetActorRotation()).GetNormalized();
}

void UEUCharacterAnimInstance::HitBy(const AActor* DamageCauser)
{
	EUCHECK(HitReactionMontage != nullptr);
	EUCHECK(DamageCauser != nullptr);

	FVector ForwardVector = EUCharacter->GetActorForwardVector();
	FVector ToDamageCauser = DamageCauser->GetActorLocation() - EUCharacter->GetActorLocation();
	ToDamageCauser.Z = 0.0f;
	ToDamageCauser.Normalize();
	
	float DotResult = FVector::DotProduct(ForwardVector, ToDamageCauser);
	int32 SectionIndex = 0;

	Montage_Play(HitReactionMontage);

	// The 'DamageCauser' is in front of the current character.
	// DotResult > sqrt(2)/2
	if (DotResult > 0.707106f)
	{
		EULOG(Warning, TEXT("Front"));
		Montage_JumpToSection(TEXT("Front"), HitReactionMontage);
	}
	// behind of the current character.
	else if (DotResult < -0.707106f)
	{
		EULOG(Warning, TEXT("Back"));
		Montage_JumpToSection(TEXT("Back"), HitReactionMontage);
	}
	else
	{
		// on the right.
		if (FVector::CrossProduct(ForwardVector, ToDamageCauser).Z > 0.0f)
		{
			EULOG(Warning, TEXT("Right"));
			Montage_JumpToSection(TEXT("Right"), HitReactionMontage);
		}
		// on the left.
		else
		{
			EULOG(Warning, TEXT("Left"));
			Montage_JumpToSection(TEXT("Left"), HitReactionMontage);
		}
	}
}

void UEUCharacterAnimInstance::PlayAttackMontage()
{
	EUCHECK(AttackMontage != nullptr);
	EUCHECK(!bIsDead);
	
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
