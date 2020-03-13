// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "EUGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FEUCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FEUCharacterData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UEUGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UEUGameInstance();

	virtual void Init() override;
	const FEUCharacterData* GetPlayerCharacterData(int32 Level);
	const FEUCharacterData* GetEnemyCharacterData(int32 Level);

private:
	UPROPERTY()
	UDataTable* EUPlayerCharacterData;

	UPROPERTY()
	UDataTable* EUEnemyCharacterData;
};
