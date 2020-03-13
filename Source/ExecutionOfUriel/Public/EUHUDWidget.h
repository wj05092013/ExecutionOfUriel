// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "Blueprint/UserWidget.h"
#include "EUHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UEUHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void BindPlayerState(class AEUPlayerState* InPlayerState);
	void BindCharacterStatus(class UEUCharacterStatusComponent* InCharacterStatus);

	void UpdateLevel();
	void UpdateName();
	void UpdateHP();
	void UpdateMP();
	void UpdateExp();

private:
	TWeakObjectPtr<class AEUPlayerState> PlayerState;
	TWeakObjectPtr<class UEUCharacterStatusComponent> CharacterStatus;

	UPROPERTY()
	class UTextBlock* TBLevel;

	UPROPERTY()
	class UTextBlock* TBName;

	UPROPERTY()
	class UProgressBar* PBHP;

	UPROPERTY()
	class UProgressBar* PBMP;

	UPROPERTY()
	class UProgressBar* PBExp;
};
