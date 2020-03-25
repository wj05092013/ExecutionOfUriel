// Fill out your copyright notice in the Description page of Project Settings.


#include "EUCharacter.h"
#include "EUCharacterAnimInstance.h"
#include "EUEnemyAIController.h"
#include "EUCharacterStatusComponent.h"
#include "Components/WidgetComponent.h"
#include "EUCharacterInfoWidget.h"
#include "EUDamageWidget.h"

AEUCharacter::AEUCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterObjectState = ECharacterObjectState::PreInit;

	CharacterStatus = CreateDefaultSubobject<UEUCharacterStatusComponent>(TEXT("CharacterStatus"));

	// UI settings
	//

	CharacterInfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CharacterInfoWidget"));
	CharacterInfoWidget->SetupAttachment(GetMesh());
	
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_CHARACTER_INFO(TEXT("/Game/ExecutionOfUriel/UI/UI_CharacterInfo.UI_CharacterInfo_C"));
	EUCHECK(UI_CHARACTER_INFO.Succeeded());
	CharacterInfoWidget->SetWidgetClass(UI_CHARACTER_INFO.Class);

	CharacterInfoWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	CharacterInfoWidget->SetDrawSize(FVector2D(120.0f, 40.0f));
	CharacterInfoWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

void AEUCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterObjectState = ECharacterObjectState::Ready;

	Anim = Cast<UEUCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	EUCHECK(Anim != nullptr);

	Anim->OnAttack.AddUObject(this, &AEUCharacter::OnAttack);
	Anim->OnMontageEnded.AddDynamic(this, &AEUCharacter::OnMontageEnded);

	CharacterStatus->OnDead.AddUObject(this, &AEUCharacter::OnDead);
}

void AEUCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CharacterObjectState = ECharacterObjectState::Alive;

	auto CharacterInfoWidgetObject = Cast<UEUCharacterInfoWidget>(CharacterInfoWidget->GetUserWidgetObject());
	EUCHECK(CharacterInfoWidgetObject != nullptr);
	CharacterInfoWidgetObject->BindCharacterStatus(CharacterStatus);
}

void AEUCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ECharacterObjectState AEUCharacter::GetCharacterObjectState() const
{
	return CharacterObjectState;
}

void AEUCharacter::OnDead()
{
	SetCanBeDamaged(false);
	SetActorEnableCollision(false);

	Anim->SetDead();

	CharacterInfoWidget->SetVisibility(false);

	EULOG_S(Warning);

	CharacterObjectState = ECharacterObjectState::Dead;
}

void AEUCharacter::SpawnDamageWidget(int32 DamageAmount)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	EUCHECK(PlayerController != nullptr);

	auto Widget = Cast<UEUDamageWidget>(CreateWidget(PlayerController, DamageWidgetClass));
	EUCHECK(Widget != nullptr);

	Widget->InitializeDamage(this, DamageAmount);
	Widget->AddToViewport();
}

void AEUCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == Anim->GetAttackMontage())
	{
		OnAttackEnd.Broadcast();
	}
}

float AEUCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	Attacker = EventInstigator;
	CharacterStatus->AdjustHP(-FinalDamage);

	SpawnDamageWidget(FinalDamage);

	return FinalDamage;
}

float AEUCharacter::GetAttackRange() const
{
	return AttackRangeDepth;
}
