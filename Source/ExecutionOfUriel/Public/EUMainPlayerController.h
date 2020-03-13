// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "GameFramework/PlayerController.h"
#include "EUMainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API AEUMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AEUMainPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UEUHUDWidget> HUDWidgetClass;

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
	class UEUHUDWidget* HUDWidget;
};
