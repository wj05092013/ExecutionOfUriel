// Fill out your copyright notice in the Description page of Project Settings.


#include "EUCharacterInfoWidget.h"
#include "EUCharacterStatusComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UEUCharacterInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TBLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Level")));
	EUCHECK(TBLevel != nullptr);

	TBName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Name")));
	EUCHECK(TBName != nullptr);

	PBHP = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	EUCHECK(PBHP != nullptr);

	PBMP = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_MPBar")));
	EUCHECK(PBMP != nullptr);

	SetCharacterName();
	OnLevelChanged();
	OnHPChanged();
	OnMPChanged();
}

void UEUCharacterInfoWidget::BindCharacterStatus(UEUCharacterStatusComponent* NewCharacterStatus)
{
	EUCHECK(NewCharacterStatus != nullptr);

	NewCharacterStatus->OnLevelChanged.AddUObject(this, &UEUCharacterInfoWidget::OnLevelChanged);
	NewCharacterStatus->OnHPChanged.AddUObject(this, &UEUCharacterInfoWidget::OnHPChanged);
	NewCharacterStatus->OnMPChanged.AddUObject(this, &UEUCharacterInfoWidget::OnMPChanged);

	CharacterStatus = NewCharacterStatus;
}

void UEUCharacterInfoWidget::SetCharacterName()
{
	EUCHECK(IsValid(CharacterStatus));

	TBName->SetText(FText::FromString(CharacterStatus->CharacterName));
}

void UEUCharacterInfoWidget::OnLevelChanged()
{
	EUCHECK(IsValid(CharacterStatus));

	TBLevel->SetText(FText::FromString(FString::FromInt(CharacterStatus->GetLevel())));
}

void UEUCharacterInfoWidget::OnHPChanged()
{
	EUCHECK(IsValid(CharacterStatus));

	PBHP->SetPercent(CharacterStatus->GetHPRatio());
}

void UEUCharacterInfoWidget::OnMPChanged()
{
	EUCHECK(IsValid(CharacterStatus));

	PBMP->SetPercent(CharacterStatus->GetMPRatio());
}
