// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "GameFramework/PlayerState.h"
#include "EUPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLevelChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExpChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPlayerNameChangedDelegate);

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	// =============================================================================
	// States
public:
	AEUPlayerState();

	void Initialize();

	int32 GetLevel() const { return Level; }
	void LevelUp();

	int32 GetExp() const { return Exp; }
	float GetExpRatio() const;
	void AdjustExp(int32 InputExp);

	virtual void SetPlayerName(const FString& NewName) override;

private:
	void SetLevel(int32 NewLevel);

	// =============================================================================
	// Delegates
public:
	FOnLevelChangedDelegate OnLevelChanged;
	FOnExpChangedDelegate OnExpChanged;
	FOnPlayerNameChangedDelegate OnPlayerNameChanged;

private:
	struct FEUCharacterData const* CurrentData = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 Exp;
};
