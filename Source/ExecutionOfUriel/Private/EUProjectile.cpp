// Fill out your copyright notice in the Description page of Project Settings.


#include "EUProjectile.h"
#include "EUEnemyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

AEUProjectile::AEUProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components settings
	//
	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	FlyingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlyingEffect"));
	ExplosionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffect"));

	RootComponent = Collision;
	FlyingEffect->SetupAttachment(RootComponent);
	ExplosionEffect->SetupAttachment(RootComponent);

	Collision->SetCollisionProfileName(TEXT("CharacterSkill"));
	
	Collision->SetSphereRadius(15.0f);

	// Movement settings
	//

	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 4000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bForceSubStepping = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	

	// Effect settings
	//

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_FLYING(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBall.P_ky_waterBall"));
	EUCHECK(P_FLYING.Succeeded());

	FlyingEffect->SetTemplate(P_FLYING.Object);
	FlyingEffect->bAutoActivate = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_EXPLOSION(TEXT("/Game/FXVarietyPack/Particles/P_ky_waterBallHit.P_ky_waterBallHit"));
	EUCHECK(P_EXPLOSION.Succeeded());

	ExplosionEffect->SetTemplate(P_EXPLOSION.Object);
	ExplosionEffect->bAutoActivate = false;

	bSetDestroy = false;
}

void AEUProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AEUProjectile::OnEnemyCharacterOverlap);
}

void AEUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	StartPos = GetActorLocation();
}

void AEUProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	auto OwnerCharacter = Cast<AEUCharacter>(GetOwner());
	EUCHECK(OwnerCharacter != nullptr);
	if (FVector::Dist(StartPos, GetActorLocation()) > OwnerCharacter->GetAttackRange())
		StartDestroy();
}

void AEUProjectile::Fire(const FVector& ShootDirection, TWeakObjectPtr<USceneComponent> TargetComp, float AttackDamage)
{
	ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	
	Damage = AttackDamage;

	if (TargetComp.IsValid())
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = TargetComp;
		ProjectileMovement->HomingAccelerationMagnitude = 50000.0f;
	}
}

void AEUProjectile::OnEnemyCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSeep, const FHitResult& SweepResult)
{
	// Something is on the way of this object, then destroy this.
	StartDestroy();

	auto Enemy = Cast<AEUEnemyCharacter>(OtherActor);
	if (Enemy != nullptr)
	{
		// Apply damage to the victim.
		//

		auto OwnerCharacter = Cast<AEUCharacter>(GetOwner());
		EUCHECK(OwnerCharacter != nullptr);

		auto Controller = OwnerCharacter->GetController();
		EUCHECK(Controller != nullptr);

		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(Damage, DamageEvent, Controller, this);
	}
}

void AEUProjectile::OnExplosionEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	ExplosionEffect->Deactivate();
	Destroy();
}

void AEUProjectile::StartDestroy()
{
	if (bSetDestroy)
		return;

	SetActorEnableCollision(false);
	ProjectileMovement->Deactivate();

	FlyingEffect->Deactivate();
	ExplosionEffect->Activate();
	ExplosionEffect->OnSystemFinished.AddDynamic(this, &AEUProjectile::OnExplosionEffectFinished);

	bSetDestroy = true;
}