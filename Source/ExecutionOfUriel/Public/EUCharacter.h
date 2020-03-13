// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "GameFramework/Character.h"
#include "EUCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UENUM(BlueprintType)
enum class ECharacterObjectState : uint8
{
	PreInit,
	Ready,
	Alive,
	Dead
};

UCLASS()
class EXECUTIONOFURIEL_API AEUCharacter : public ACharacter
{
	GENERATED_BODY()
	
	// =============================================================================
	// Initialization
public:
	AEUCharacter();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// =============================================================================
	// State

	ECharacterObjectState GetCharacterObjectState() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterObjectState CharacterObjectState;

	// =============================================================================
	// Status
public:
	virtual void OnDead();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	class UEUCharacterStatusComponent* CharacterStatus;

	// =============================================================================
	// UI
private:
	void SpawnDamageWidget(int32 DamageAmount);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UWidgetComponent* CharacterInfoWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<class UEUDamageWidget> DamageWidgetClass;

	// =============================================================================
	// Animation
public:
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY()
	class UEUCharacterAnimInstance* Anim;
	
	// =============================================================================
	// Combat
public:
	virtual void Attack() {};
	virtual void OnAttack() {};

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	float GetAttackRange() const;

	FOnAttackEndDelegate OnAttackEnd;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, Meta = (ClampMin = 0.0))
	float AttackRangeDepth;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Combat)
	TWeakObjectPtr<AController> Attacker;
};
