// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "Blueprint/UserWidget.h"
#include "EUCharacterInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UEUCharacterInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void BindCharacterStatus(class UEUCharacterStatusComponent* NewCharacterStatus);

	void SetCharacterName();
	void OnLevelChanged();
	void OnHPChanged();
	void OnMPChanged();

private:
	class UEUCharacterStatusComponent* CharacterStatus;

	UPROPERTY()
	class UTextBlock* TBLevel;

	UPROPERTY()
	class UTextBlock* TBName;

	UPROPERTY()
	class UProgressBar* PBHP;

	UPROPERTY()
	class UProgressBar* PBMP;
};
