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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pawn")
	class AEUCharacter* EUCharacter;

	// =============================================================================
	// Locomotion
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FVector Velocity;

	// =============================================================================
	// Dead Animation
public:
	void SetDead() { bIsDead = true; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsDead;

	// =============================================================================
	// Hit Reaction Animation
public:
	void HitBy(const AActor* DamageCauser);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit Reaction")
	UAnimMontage* HitReactionMontage;

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
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* AttackMontage;

	// This property has to be set on derived class' constructor.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Attack)
	TArray<FName> AttackMontageSectionNames;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	FRotator AimRotation;
};
