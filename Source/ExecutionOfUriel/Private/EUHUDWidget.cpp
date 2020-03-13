// Fill out your copyright notice in the Description page of Project Settings.


#include "EUHUDWidget.h"
#include "EUPlayerState.h"
#include "EUCharacterStatusComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UEUHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TBLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Level")));
	EUCHECK(TBLevel != nullptr);

	TBName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Name")));
	EUCHECK(TBName != nullptr);

	PBHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HP")));
	EUCHECK(PBHP != nullptr);

	PBMP = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_MP")));
	EUCHECK(PBMP != nullptr);

	PBExp = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_Exp")));
	EUCHECK(PBExp != nullptr);
}

void UEUHUDWidget::BindPlayerState(class AEUPlayerState* InPlayerState)
{
	EUCHECK(InPlayerState != nullptr);
	PlayerState = InPlayerState;
	
	PlayerState->OnLevelChanged.AddUObject(this, &UEUHUDWidget::UpdateLevel);
	PlayerState->OnPlayerNameChanged.AddUObject(this, &UEUHUDWidget::UpdateName);
	PlayerState->OnExpChanged.AddUObject(this, &UEUHUDWidget::UpdateExp);
}

void UEUHUDWidget::BindCharacterStatus(class UEUCharacterStatusComponent* InCharacterStatus)
{
	EUCHECK(InCharacterStatus != nullptr);
	CharacterStatus = InCharacterStatus;

	CharacterStatus->OnHPChanged.AddUObject(this, &UEUHUDWidget::UpdateHP);
	CharacterStatus->OnMPChanged.AddUObject(this, &UEUHUDWidget::UpdateMP);
}

void UEUHUDWidget::UpdateLevel()
{
	EUCHECK(PlayerState.IsValid());
	TBLevel->SetText(FText::FromString(FString::FromInt(PlayerState->GetLevel())));
}

void UEUHUDWidget::UpdateName()
{
	EUCHECK(PlayerState.IsValid());
	TBName->SetText(FText::FromString(PlayerState->GetPlayerName()));
}

void UEUHUDWidget::UpdateHP()
{
	EUCHECK(CharacterStatus.IsValid());
	PBHP->SetPercent(CharacterStatus->GetHPRatio());
}

void UEUHUDWidget::UpdateMP()
{
	EUCHECK(CharacterStatus.IsValid());
	PBMP->SetPercent(CharacterStatus->GetMPRatio());
}

void UEUHUDWidget::UpdateExp()
{
	EUCHECK(PlayerState.IsValid());
	PBExp->SetPercent(PlayerState->GetExpRatio());
}