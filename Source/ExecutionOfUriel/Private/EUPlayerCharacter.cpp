// Fill out your copyright notice in the Description page of Project Settings.


#include "EUPlayerCharacter.h"
#include "Engine/SkeletalMesh.h"
#include "Math/BoxSphereBounds.h"
#include "EUPlayerAnimInstance.h"
#include "EUCharacterStatusComponent.h"
#include "EUPlayerState.h"
#include "Components/WidgetComponent.h"
#include "EUDamageWidget.h"

AEUPlayerCharacter::AEUPlayerCharacter()
{
	/*
	Mesh and anim instance has to be set on derived class.
	*/

	// Create and attach components.
	//

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// Camera settings
	//

	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	SpringArm->TargetArmLength = 400.0f;
	ArmLengthSpeed = 20.0f;
	MinArmLength = 150.0f;
	MaxArmLength = 600.0f;

	// Movement settings
	//

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// Collision settings
	//

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EUPlayerCharacter"));

	// Combat settings
	bIsAttacking = false;
	SetComboStateAttackEnd();

	// UI settings
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_DAMAGE(TEXT("/Game/ExecutionOfUriel/UI/UI_PlayerDamage.UI_PlayerDamage_C"));
	EUCHECK(UI_DAMAGE.Succeeded());
	DamageWidgetClass = UI_DAMAGE.Class;
}

void AEUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AEUPlayerCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("LevelUp"), EInputEvent::IE_Pressed, this, &AEUPlayerCharacter::LevelUp);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AEUPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AEUPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AEUPlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AEUPlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &AEUPlayerCharacter::Zoom);
}

void AEUPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Mesh and capsule settings
	// Fit capsule size to the skeletal mesh.
	//

	auto SkeletalMesh = GetMesh()->SkeletalMesh;
	EUCHECK(SkeletalMesh != nullptr);

	FVector MeshExtent = SkeletalMesh->GetBounds().BoxExtent;
	float MeshHalfHeight = MeshExtent.X;
	float MeshRadius = MeshExtent.Y;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -MeshHalfHeight));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetCapsuleComponent()->SetCapsuleSize(CapsuleRadiusRatio * MeshRadius, MeshHalfHeight);

	// Camera settings
	//

	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 1.5f * MeshHalfHeight));

	// Animation settings
	//

	auto PlayerAnim = Cast<UEUPlayerAnimInstance>(Anim);
	EUCHECK(PlayerAnim != nullptr);
	PlayerAnim->OnCheckCombo.AddUObject(this, &AEUPlayerCharacter::OnCheckCombo);
	
	MaxCombo = PlayerAnim->MontageSectionNum() - 1;
}

void AEUPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EUPlayerState = Cast<AEUPlayerState>(GetPlayerState());
	EUCHECK(EUPlayerState != nullptr);
	EUPlayerState->OnLevelChanged.AddUObject(this, &AEUPlayerCharacter::OnLevelChanged);
	EUPlayerState->OnPlayerNameChanged.AddUObject(this, &AEUPlayerCharacter::OnPlayerNameChanged);
}

void AEUPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEUPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEUPlayerCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}

void AEUPlayerCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void AEUPlayerCharacter::Zoom(float NewAxisValue)
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + ArmLengthSpeed * NewAxisValue, MinArmLength, MaxArmLength);
}

void AEUPlayerCharacter::MoveForward(float NewAxisValue)
{
	// Movement of Z direction cannot affect to the result movement.
	FVector Direction = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);
	Direction.Z = 0.0f;
	Direction.Normalize();
	AddMovementInput(Direction, NewAxisValue);
}

void AEUPlayerCharacter::MoveRight(float NewAxisValue)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
}

void AEUPlayerCharacter::OnDead()
{
	Super::OnDead();

	DisableInput(Cast<APlayerController>(Controller));
}

void AEUPlayerCharacter::OnLevelChanged()
{
	CharacterStatus->SetLevel(EUPlayerState->GetLevel(), ECharacterType::Player);
}

void AEUPlayerCharacter::OnPlayerNameChanged()
{
	CharacterStatus->CharacterName = EUPlayerState->GetPlayerName();
}

void AEUPlayerCharacter::LevelUp()
{
	EUPlayerState->LevelUp();
}

void AEUPlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Super::OnMontageEnded(Montage, bInterrupted);

	if (Montage == Anim->GetAttackMontage())
	{
		SetComboStateAttackEnd();
		bIsAttacking = false;
	}
}

void AEUPlayerCharacter::Attack()
{
	Super::Attack();

	if (bIsAttacking)
	{
		if (bEnableComboInput)
		{
			bEnableComboInput = false;

			if (CurrentCombo < MaxCombo)
			{
				bComboInputOn = true;
			}
		}
	}
	// This input is the first.
	else
	{
		EUCHECK(CurrentCombo == 0);

		bEnableComboInput = true;
		bComboInputOn = false;
		bIsAttacking = true;

		Anim->PlayAttackMontage();
	}
}

int32 AEUPlayerCharacter::GetCurrentCombo() const
{
	return CurrentCombo;
}

void AEUPlayerCharacter::OnCheckCombo()
{
	if (bComboInputOn)
	{
		bComboInputOn = false;
		bEnableComboInput = true;

		Anim->JumpToAttackMontageSection(++CurrentCombo);
	}
}

void AEUPlayerCharacter::SetComboStateAttackEnd()
{
	CurrentCombo = 0;
	bEnableComboInput = false;
	bComboInputOn = false;
}
