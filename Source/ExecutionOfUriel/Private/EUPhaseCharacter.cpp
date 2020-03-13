// Fill out your copyright notice in the Description page of Project Settings.


#include "EUPhaseCharacter.h"
#include "EUPlayerAnimInstance.h"
#include "EUProjectile.h"
#include "DrawDebugHelpers.h"
#include "EUMath.h"
#include "EUCharacterStatusComponent.h"

const FName AEUPhaseCharacter::LeftHandSocketName(TEXT("FX_Hand_L4"));
const FName AEUPhaseCharacter::RightHandSocketName(TEXT("FX_Hand_R4"));

AEUPhaseCharacter::AEUPhaseCharacter()
{
	// Mesh loading and capsule settings
	//

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MESH(TEXT("/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC"));
	EUCHECK(SK_MESH.Succeeded())
	GetMesh()->SetSkeletalMesh(SK_MESH.Object);

	CapsuleRadiusRatio = 0.4f;

	// Movement settings
	//

	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 400.0f;

	// Animation settings
	//

	static ConstructorHelpers::FClassFinder<UAnimInstance> ANIM(TEXT("/Game/ExecutionOfUriel/Animations/Phase_AnimBlueprint.Phase_AnimBlueprint_C"));
	EUCHECK(ANIM.Succeeded())
	GetMesh()->SetAnimInstanceClass(ANIM.Class);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Combat settings
	//

	AttackRangeDepth = 2000.0f;
	TargetingRadius = 10.0f;

	ProjectileClass = AEUProjectile::StaticClass();

	// Effect settings
	//

	LeftHandFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftHandFlashEffect"));
	LeftHandFlashEffect->SetupAttachment(GetMesh(), LeftHandSocketName);

	RightHandFlashEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightHandFlashEffect"));
	RightHandFlashEffect->SetupAttachment(GetMesh(), RightHandSocketName);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HAND_FLASH(TEXT("/Game/ParagonPhase/FX/Particles/P_Phase_HandFlash.P_Phase_HandFlash"));
	EUCHECK(P_HAND_FLASH.Succeeded());
	
	LeftHandFlashEffect->SetTemplate(P_HAND_FLASH.Object);
	LeftHandFlashEffect->bAutoActivate = false;

	RightHandFlashEffect->SetTemplate(P_HAND_FLASH.Object);
	RightHandFlashEffect->bAutoActivate = false;
}

void AEUPhaseCharacter::OnAttack()
{
	Super::OnAttack();

	EUCHECK(ProjectileClass != nullptr);

	// -----------------------------------------------------------------------------
	// Ÿ�� Ž�� (�� ������ X�� ���� ������Ʈ Picking)

	TWeakObjectPtr<USceneComponent> TargetComp = nullptr;

	// ĳ������ ���� X���� �������� �ϴ� ���� ���� ���
	FPlane CharacterPlane(GetActorLocation(), GetActorForwardVector());
	// �� ���� X�� ������ ���� ǥ��
	FVector ControlDirection = GetControlRotation().Vector();

	// Ž�� ������: ControlPlane�� ControlDirection ������ ����
	FVector Start = TargetingRadius * ControlDirection
		+ FMath::RayPlaneIntersection(Camera->GetComponentLocation(), ControlDirection, CharacterPlane);
	// Ž�� ������: ControlDirection ��������, CharacterPlane���� AttackRangeDepth(���� ���� ����)��ŭ ������ ����
	FVector End = Start + (AttackRangeDepth - 2.0f * TargetingRadius) * ControlDirection;

	FHitResult HitResult;
	// Ž�� ���������� ���������� Sweep�� �����Ѵ�.
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(TargetingRadius)
	);
	// Ÿ�� Ž�� ����
	if (bResult)
	{
		TargetComp = HitResult.Component.Get();
	}

	// -----------------------------------------------------------------------------
	// �������� ���� ����

	FVector SpawnLocation;
	// Ÿ���� �������� ������ �������� ĳ���� ���� �������� ���ư���.
	FVector ShootDirection(1.0f, 0.0f, 0.0f);

	// Ÿ���� �����ϸ� �������� ��� �׸��� ���ư���.
	if (TargetComp != nullptr)
	{
		// X���� �߽����� �� �� ������, �߽� ���� 30���� ȣ�� ������ ���� ��ȯ�Ѵ�.
		FVector2D RandVec = FEUMath::RandPointInUnitArcAroundX(30.0f);
		ShootDirection = FVector(2.0f, RandVec.X, RandVec.Y);
	}

	// ������ �߻� ��ġ�� �޼����� ����
	if (GetCurrentCombo() & 1)
	{
		LeftHandFlashEffect->Activate(true);
		SpawnLocation = GetMesh()->GetSocketLocation(LeftHandSocketName) + 10.0f * GetActorForwardVector();
		// ������ �߻簡 �⺻�̹Ƿ�, �߻� ������ Y���� ������Ų��.
		ShootDirection.Y = -ShootDirection.Y;
	}
	// ������ �߻� ��ġ�� ���������� ����
	else
	{
		RightHandFlashEffect->Activate(true);
		SpawnLocation = GetMesh()->GetSocketLocation(RightHandSocketName) + 10.0f * GetActorForwardVector();
	}

	// -----------------------------------------------------------------------------
	// ������(UProjectileMovementComponent ����) ���� ����

	// �������� ������(���� ������)�� �����Ѵ�.
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;

	// AEUProjectile Ŭ���� ������ ���� ��ü ����
	AEUProjectile* Projectile = GetWorld()->SpawnActor<AEUProjectile>(ProjectileClass, SpawnLocation, GetActorRotation(), SpawnParam);
	if (Projectile != nullptr)
	{
		ShootDirection.Normalize();
		// ���� �������� �߻縦 �����Ѵ�.
		Projectile->Fire(GetActorRotation().RotateVector(ShootDirection), TargetComp, CharacterStatus->GetMagic());
	}
}
