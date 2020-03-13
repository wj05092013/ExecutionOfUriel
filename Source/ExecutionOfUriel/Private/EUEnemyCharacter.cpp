// Fill out your copyright notice in the Description page of Project Settings.


#include "EUEnemyCharacter.h"
#include "EUEnemyAIController.h"
#include "EUEnemyAnimInstance.h"
#include "EUCharacterStatusComponent.h"
#include "EUPlayerCharacter.h"
#include "EUPlayerState.h"
#include "EUDamageWidget.h"

AEUEnemyCharacter::AEUEnemyCharacter()
{
	// AI settings
	AIControllerClass = AEUEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Movement settings
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);

	// Collision settings
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EUEnemyCharacter"));

	// Status settings
	Level = 1;
	DeadTimer = 5.0f;

	// Combat settings
	bIsAttacking = false;

	// UI settings
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_DAMAGE(TEXT("/Game/ExecutionOfUriel/UI/UI_EnemyDamage.UI_EnemyDamage_C"));
	EUCHECK(UI_DAMAGE.Succeeded());
	DamageWidgetClass = UI_DAMAGE.Class;
}

void AEUEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStatus->SetLevel(Level, ECharacterType::Enemy);
}

void AEUEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEUEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEUEnemyAIController>(GetController());
	EnemyAIController->RunAI();
}

void AEUEnemyCharacter::OnDead()
{
	Super::OnDead();

	// Stop conroller of this pawn.
	auto AIController = Cast<AEUEnemyAIController>(Controller);
	EUCHECK(AIController != nullptr);
	AIController->StopAI();

	// Deliver DropExp value to 'Attacker', the player who has attacked this pawn.
	EUCHECK(Attacker.IsValid());
	auto PlayerCharacter = Cast<AEUPlayerCharacter>(Attacker->GetPawn());
	EUCHECK(PlayerCharacter != nullptr);
	auto EUPlayerState = Cast<AEUPlayerState>(PlayerCharacter->GetPlayerState());
	EUCHECK(EUPlayerState != nullptr);
	EUPlayerState->AdjustExp(CharacterStatus->GetDropExp());

	// This pawn will be destroyed when this timer expires.
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[this]()->void {
			Destroy();
		}
	), DeadTimer, false);
}

void AEUEnemyCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);

	if (Montage == Anim->GetAttackMontage())
	{
		bIsAttacking = false;
	}
}

void AEUEnemyCharacter::Attack()
{
	Super::Attack();

	if (!bIsAttacking)
	{
		int32 SectionIndex = FMath::RandRange(0, Anim->MontageSectionNum() - 1);

		Anim->PlayAttackMontage();
		Anim->JumpToAttackMontageSection(SectionIndex);

		bIsAttacking = true;
	}
}

float AEUEnemyCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	auto EUController = Cast<AEUEnemyAIController>(GetController());
	EUCHECK(EUController != nullptr, FinalDamage);

	EUCHECK(Attacker != nullptr, FinalDamage);
	auto AttackerPawn = Cast<AEUCharacter>(Attacker->GetPawn());
	EUCHECK(AttackerPawn != nullptr, FinalDamage);
	EUController->HitBy(AttackerPawn);

	return FinalDamage;
}