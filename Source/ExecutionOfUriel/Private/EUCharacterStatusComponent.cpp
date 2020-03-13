// Fill out your copyright notice in the Description page of Project Settings.


#include "EUCharacterStatusComponent.h"
#include "EUGameInstance.h"
#include "EUCharacter.h"

UEUCharacterStatusComponent::UEUCharacterStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}

void UEUCharacterStatusComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEUCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEUCharacterStatusComponent::SetLevel(int32 NewLevel, ECharacterType Type)
{
	auto EUGameInstance = Cast<UEUGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	EUCHECK(EUGameInstance != nullptr);

	CurrentData = (Type == ECharacterType::Player) ? (EUGameInstance->GetPlayerCharacterData(NewLevel)) : (EUGameInstance->GetEnemyCharacterData(NewLevel));
	EUCHECK(CurrentData != nullptr);

	Level = NewLevel;

	// Recover HP and MP when the character level up.
	AdjustHP(CurrentData->MaxHP);
	AdjustMP(CurrentData->MaxMP);

	OnLevelChanged.Broadcast();
}

float UEUCharacterStatusComponent::GetHPRatio() const
{
	EUCHECK(CurrentData != nullptr, 0.0f);
	if(CurrentData->MaxHP <= 0)
		return 0.0f;
	return static_cast<float>(HP) / static_cast<float>(CurrentData->MaxHP);
}

void UEUCharacterStatusComponent::AdjustHP(int32 InputHP)
{
	EUCHECK(CurrentData != nullptr);

	HP = FMath::Clamp(HP + InputHP, 0, CurrentData->MaxHP);

	if (HP == 0)
		OnDead.Broadcast();

	OnHPChanged.Broadcast();
}

float UEUCharacterStatusComponent::GetMPRatio() const
{
	EUCHECK(CurrentData != nullptr, 0.0f);
	if (CurrentData->MaxHP <= 0)
		return 0.0f;
	return static_cast<float>(MP) / static_cast<float>(CurrentData->MaxMP);
}

void UEUCharacterStatusComponent::AdjustMP(int32 InputMP)
{
	EUCHECK(CurrentData != nullptr);

	MP += InputMP;
	FMath::Clamp(MP, 0, CurrentData->MaxMP);

	OnMPChanged.Broadcast();
}

int32 UEUCharacterStatusComponent::GetAttack() const
{
	EUCHECK(CurrentData != nullptr, 0);
	return CurrentData->Attack;
}

int32 UEUCharacterStatusComponent::GetMagic() const
{
	EUCHECK(CurrentData != nullptr, 0);
	return CurrentData->Magic;
}

int32 UEUCharacterStatusComponent::GetDropExp() const
{
	EUCHECK(CurrentData != nullptr, 0);
	return CurrentData->DropExp;
}