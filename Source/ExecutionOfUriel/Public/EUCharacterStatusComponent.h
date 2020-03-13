// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "Components/ActorComponent.h"
#include "EUCharacterStatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLevelUpdatedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMPChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDeadDelegate);

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player,
	Enemy
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTIONOFURIEL_API UEUCharacterStatusComponent : public UActorComponent
{
	GENERATED_BODY()

	// =============================================================================
	// Initialization
public:
	UEUCharacterStatusComponent();
	virtual void InitializeComponent() override;

protected:
	virtual void BeginPlay() override;

	// =============================================================================
	// Delegates
public:
	FOnLevelUpdatedDelegate OnLevelChanged;
	FOnHPChangedDelegate OnHPChanged;
	FOnMPChangedDelegate OnMPChanged;
	FOnDeadDelegate OnDead;

	// =============================================================================
	// States
public:
	int32 GetLevel() const { return Level; };
	void SetLevel(int32 NewLevel, ECharacterType Type);

	// =============================================================================
	// Status

	int32 GetHP() const { return HP; };
	float GetHPRatio() const;
	void AdjustHP(int32 InputMP);

	int32 GetMP() const { return MP; };
	float GetMPRatio() const;
	void AdjustMP(int32 InputHP);

	int32 GetAttack() const;
	int32 GetMagic() const;

	int32 GetDropExp() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	FString CharacterName;

private:
	struct FEUCharacterData const* CurrentData = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	int32 HP;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Status, Meta = (AllowPrivateAccess = true))
	int32 MP;
};
