// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "GameFramework/HUD.h"
#include "EUHUD.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = CrossHair)
	UTexture2D* CrossHairTex;
};
