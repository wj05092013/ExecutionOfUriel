// Fill out your copyright notice in the Description page of Project Settings.


#include "EUMainPlayerController.h"
#include "EUCharacter.h"
#include "EUPlayerState.h"
#include "EUHUDWidget.h"

AEUMainPlayerController::AEUMainPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/ExecutionOfUriel/UI/UI_HUD.UI_HUD_C"));
	EUCHECK(UI_HUD.Succeeded());
	HUDWidgetClass = UI_HUD.Class;
}

void AEUMainPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEUMainPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	HUDWidget = CreateWidget<UEUHUDWidget>(this, HUDWidgetClass);
	EUCHECK(HUDWidget != nullptr);
	HUDWidget->AddToViewport();

	auto EUPlayerState = Cast<AEUPlayerState>(PlayerState);
	EUCHECK(EUPlayerState != nullptr);
	HUDWidget->BindPlayerState(EUPlayerState);

	auto EUCharacter = Cast<AEUCharacter>(InPawn);
	EUCHECK(EUCharacter != nullptr);
	HUDWidget->BindCharacterStatus(EUCharacter->CharacterStatus);
}

void AEUMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
}