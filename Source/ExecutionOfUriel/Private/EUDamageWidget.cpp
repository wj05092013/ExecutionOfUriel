// Fill out your copyright notice in the Description page of Project Settings.


#include "EUDamageWidget.h"
#include "Components/TextBlock.h"
#include "EUCharacter.h"

void UEUDamageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TBDamage = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_Damage")));
	EUCHECK(TBDamage != nullptr);
	
	TBDamage->SetText(FText::FromString(FString::FromInt(DamageAmount)));

	PlayerController = GetWorld()->GetFirstPlayerController();
	EUCHECK(PlayerController != nullptr);
	EUCHECK(VictimCharacter != nullptr);

	StartPosition = VictimCharacter->GetActorLocation();
	StartPosition.Z += VictimCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 100.0f;
	EndPosition = StartPosition;
	EndPosition.Z += 100.0f;

	FVector2D StartPositionScreen;
	PlayerController->ProjectWorldLocationToScreen(StartPosition, StartPositionScreen);
	SetPositionInViewport(StartPositionScreen);

	PlayAnimation(WidgetAnim);
}

void UEUDamageWidget::InitializeDamage(const ACharacter* InVictimCharacter, int32 InDamage)
{
	VictimCharacter = InVictimCharacter;
	DamageAmount = InDamage;
}

void UEUDamageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentTimer += InDeltaTime;
	if (CurrentTimer >= DestroyTimer)
		RemoveFromViewport();
	
	EUCHECK(DestroyTimer > KINDA_SMALL_NUMBER);
	auto CurrentPosition = FMath::Lerp(StartPosition, EndPosition, CurrentTimer / DestroyTimer);

	FVector2D CurrentPositionScreen;
	PlayerController->ProjectWorldLocationToScreen(CurrentPosition, CurrentPositionScreen);
	SetPositionInViewport(CurrentPositionScreen);
}