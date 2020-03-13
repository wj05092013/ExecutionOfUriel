// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ExecutionOfUriel.h"
#include "Blueprint/UserWidget.h"
#include "EUDamageWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTIONOFURIEL_API UEUDamageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void InitializeDamage(const ACharacter* InVictimCharacter, int32 InDamage);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	class UTextBlock* TBDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = true))
	class UWidgetAnimation* WidgetAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	float DestroyTimer = 2.0f;

	float CurrentTimer = 0.0f;

	class ACharacter const* VictimCharacter = nullptr;
	class APlayerController* PlayerController = nullptr;

	FVector StartPosition;
	FVector EndPosition;
	int32 DamageAmount;
};
