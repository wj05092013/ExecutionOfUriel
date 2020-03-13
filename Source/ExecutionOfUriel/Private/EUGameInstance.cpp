// Fill out your copyright notice in the Description page of Project Settings.


#include "EUGameInstance.h"

UEUGameInstance::UEUGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PLAYER_CHARACTER_DATA(TEXT("/Game/ExecutionOfUriel/GameData/EUPlayerCharacterData.EUPlayerCharacterData"));
	EUCHECK(DT_PLAYER_CHARACTER_DATA.Succeeded());
	EUPlayerCharacterData = DT_PLAYER_CHARACTER_DATA.Object;
	EUCHECK(EUPlayerCharacterData->GetRowMap().Num() > 0);

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY_CHARACTER_DATA(TEXT("/Game/ExecutionOfUriel/GameData/EUEnemyCharacterData.EUEnemyCharacterData"));
	EUCHECK(DT_ENEMY_CHARACTER_DATA.Succeeded());
	EUEnemyCharacterData = DT_ENEMY_CHARACTER_DATA.Object;
	EUCHECK(EUEnemyCharacterData->GetRowMap().Num() > 0);
}

void UEUGameInstance::Init()
{
	Super::Init();
}

const FEUCharacterData* UEUGameInstance::GetPlayerCharacterData(int32 Level)
{
	return EUPlayerCharacterData->FindRow<FEUCharacterData>(*FString::FromInt(Level), TEXT(""));
}

const FEUCharacterData* UEUGameInstance::GetEnemyCharacterData(int32 Level)
{
	return EUEnemyCharacterData->FindRow<FEUCharacterData>(*FString::FromInt(Level), TEXT(""));
}
