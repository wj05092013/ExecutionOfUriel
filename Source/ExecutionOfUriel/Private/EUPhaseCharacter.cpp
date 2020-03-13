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
	// 타겟 탐색 (뷰 공간의 X축 방향 오브젝트 Picking)

	TWeakObjectPtr<USceneComponent> TargetComp = nullptr;

	// 캐릭터의 로컬 X축을 법선으로 하는 월드 공간 평면
	FPlane CharacterPlane(GetActorLocation(), GetActorForwardVector());
	// 뷰 공간 X축 벡터의 월드 표현
	FVector ControlDirection = GetControlRotation().Vector();

	// 탐색 시작점: ControlPlane과 ControlDirection 벡터의 교점
	FVector Start = TargetingRadius * ControlDirection
		+ FMath::RayPlaneIntersection(Camera->GetComponentLocation(), ControlDirection, CharacterPlane);
	// 탐색 종착점: ControlDirection 방향으로, CharacterPlane에서 AttackRangeDepth(공격 가능 범위)만큼 떨어진 지점
	FVector End = Start + (AttackRangeDepth - 2.0f * TargetingRadius) * ControlDirection;

	FHitResult HitResult;
	// 탐색 시작점부터 종착점까지 Sweep을 수행한다.
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start, End, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(TargetingRadius)
	);
	// 타겟 탐색 성공
	if (bResult)
	{
		TargetComp = HitResult.Component.Get();
	}

	// -----------------------------------------------------------------------------
	// 마법구의 방향 설정

	FVector SpawnLocation;
	// 타겟이 존재하지 않으면 마법구는 캐릭터 전방 직선으로 날아간다.
	FVector ShootDirection(1.0f, 0.0f, 0.0f);

	// 타겟이 존재하면 마법구는 곡선을 그리며 날아간다.
	if (TargetComp != nullptr)
	{
		// X축을 중심으로 한 원 위에서, 중심 각이 30도인 호의 임의의 점을 반환한다.
		FVector2D RandVec = FEUMath::RandPointInUnitArcAroundX(30.0f);
		ShootDirection = FVector(2.0f, RandVec.X, RandVec.Y);
	}

	// 마법구 발사 위치를 왼손으로 설정
	if (GetCurrentCombo() & 1)
	{
		LeftHandFlashEffect->Activate(true);
		SpawnLocation = GetMesh()->GetSocketLocation(LeftHandSocketName) + 10.0f * GetActorForwardVector();
		// 오른손 발사가 기본이므로, 발사 방향의 Y값을 반전시킨다.
		ShootDirection.Y = -ShootDirection.Y;
	}
	// 마법구 발사 위치를 오른손으로 설정
	else
	{
		RightHandFlashEffect->Activate(true);
		SpawnLocation = GetMesh()->GetSocketLocation(RightHandSocketName) + 10.0f * GetActorForwardVector();
	}

	// -----------------------------------------------------------------------------
	// 마법구(UProjectileMovementComponent 포함) 액터 스폰

	// 마법구의 소유자(공격 가해자)를 설정한다.
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;

	// AEUProjectile 클래스 정보를 통해 객체 스폰
	AEUProjectile* Projectile = GetWorld()->SpawnActor<AEUProjectile>(ProjectileClass, SpawnLocation, GetActorRotation(), SpawnParam);
	if (Projectile != nullptr)
	{
		ShootDirection.Normalize();
		// 실제 마법구의 발사를 수행한다.
		Projectile->Fire(GetActorRotation().RotateVector(ShootDirection), TargetComp, CharacterStatus->GetMagic());
	}
}
