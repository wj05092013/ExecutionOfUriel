// Fill out your copyright notice in the Description page of Project Settings.


#include "EUMainGameModeBase.h"
#include "EUPhaseCharacter.h"
#include "EUMainPlayerController.h"
#include "EUPlayerState.h"
#include "EUHUD.h"

AEUMainGameModeBase::AEUMainGameModeBase()
{
	DefaultPawnClass = AEUPhaseCharacter::StaticClass();
	PlayerControllerClass = AEUMainPlayerController::StaticClass();
	PlayerStateClass = AEUPlayerState::StaticClass();

	static ConstructorHelpers::FClassFinder<AHUD> BP_EUHUD(TEXT("/Game/ExecutionOfUriel/Blueprints/BP_EUHUD.BP_EUHUD_C"));
	EUCHECK(BP_EUHUD.Succeeded());
	HUDClass = BP_EUHUD.Class;
}

void AEUMainGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto EUPlayerState = Cast<AEUPlayerState>(NewPlayer->PlayerState);
	EUCHECK(EUPlayerState != nullptr);
	EUPlayerState->Initialize();
}