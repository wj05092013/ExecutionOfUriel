// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "EUCharacterAnimInstance.h"
#include "EUPlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerCheckComboDelegate);

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UEUPlayerAnimInstance : public UEUCharacterAnimInstance
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void AnimNotify_CheckCombo();

public:
	FOnPlayerCheckComboDelegate OnCheckCombo;
};
