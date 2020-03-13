// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "GameFramework/GameModeBase.h"
#include "EUMainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AEUMainGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
