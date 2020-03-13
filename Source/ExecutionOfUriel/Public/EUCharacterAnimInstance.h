// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "Animation/AnimInstance.h"
#include "EUCharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackDelegate);

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UEUCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UEUCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// =============================================================================
	// Dead Animation

	void SetDead() { bIsDead = true; }

	void PlayDeathMontage();
	const UAnimMontage* GetDeadMontage() const { return DeathMontage; }

protected:
	// This property has to be set on derived class' constructor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Death)
	UAnimMontage* DeathMontage;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;

	// =============================================================================
	// Combat Animation
public:
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 SectionIndex);

	int32 MontageSectionNum() const;
	const UAnimMontage* GetAttackMontage() const { return AttackMontage; }

private:
	UFUNCTION()
	virtual void AnimNotify_Attack();

public:
	FOnAttackDelegate OnAttack;

protected:
	// This property has to be set on derived class' constructor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack)
	UAnimMontage* AttackMontage;

	// This property has to be set on derived class' constructor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack)
	TArray<FName> AttackMontageSectionNames;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator AimRotation;
};
