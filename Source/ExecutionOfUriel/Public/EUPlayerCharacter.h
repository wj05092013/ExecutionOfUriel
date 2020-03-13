// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "EUCharacter.h"
#include "EUPlayerCharacter.generated.h"

UCLASS()
class EXECUTIONOFURIEL_API AEUPlayerCharacter : public AEUCharacter
{
	GENERATED_BODY()

public:
	AEUPlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	float CapsuleRadiusRatio = 0.0f;

	// =============================================================================
	// Camera
public:
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Zoom(float NewAxisValue);

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

protected:
	float ArmLengthSpeed = 0.0f;
	float MinArmLength = 0.0f;
	float MaxArmLength = 0.0f;

	// =============================================================================
	// Movement
public:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);

	// =============================================================================
	// Status
public:
	virtual void OnDead() override;

	// =============================================================================
	// State

	void OnLevelChanged();
	void LevelUp();

	void OnPlayerNameChanged();

private:
	class AEUPlayerState* EUPlayerState;

	// =============================================================================
	// Animation
public:
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

	// =============================================================================
	// Combat
	virtual void Attack() override;

protected:
	int32 GetCurrentCombo() const;

private:
	void OnCheckCombo();
	void SetComboStateAttackEnd();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

private:
	// Combo count range: [0, MaxCombo]
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bEnableComboInput;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool bIsAttacking;
};
