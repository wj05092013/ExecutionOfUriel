// Fill out your copyright notice in the Description page of Project Settings.


#include "EUPlayerState.h"
#include "EUGameInstance.h"

AEUPlayerState::AEUPlayerState()
{
	Level = 1;
	Exp = 0;
}

void AEUPlayerState::Initialize()
{
	SetLevel(1);
	AdjustExp(0);

	// **! Revise later.
	SetPlayerName(TEXT("Owen"));
}

void AEUPlayerState::LevelUp()
{
	EUCHECK(CurrentData != nullptr);

	if (CurrentData->NextExp != -1)
		AdjustExp(CurrentData->NextExp - Exp);
}

float AEUPlayerState::GetExpRatio() const
{
	EUCHECK(CurrentData != nullptr, 0.0f);
	EUCHECK(CurrentData->NextExp != 0, 0.0f);

	if (CurrentData->NextExp == -1)
		return 0.0f;

	return static_cast<float>(Exp) / static_cast<float>(CurrentData->NextExp);
}

void AEUPlayerState::AdjustExp(int32 InputExp)
{
	EUCHECK(CurrentData != nullptr);

	if (CurrentData->NextExp != -1)
	{
		Exp += InputExp;

		if (Exp >= CurrentData->NextExp)
		{
			Exp -= CurrentData->NextExp;
			SetLevel(Level + 1);

			EUCHECK(CurrentData != nullptr);
			if (CurrentData->NextExp == -1)
				Exp = 0;
		}

		OnExpChanged.Broadcast();
	}
}

void AEUPlayerState::SetPlayerName(const FString& NewName)
{
	Super::SetPlayerName(NewName);
	OnPlayerNameChanged.Broadcast();
}

void AEUPlayerState::SetLevel(int32 NewLevel)
{
	auto EUGameInstance = Cast<UEUGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	EUCHECK(EUGameInstance != nullptr);

	CurrentData = EUGameInstance->GetPlayerCharacterData(NewLevel);
	EUCHECK(CurrentData);

	Level = NewLevel;

	OnLevelChanged.Broadcast();
}