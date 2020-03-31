// Fill out your copyright notice in the Description page of Project Settings.


#include "EUProjectile.h"
#include "EUCharacter.h"
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
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));

	RootComponent = Collision;
	FlyingEffect->SetupAttachment(RootComponent);
	HitEffect->SetupAttachment(RootComponent);
	
	// Effect settings
	//

	FlyingEffect->bAutoActivate = true;
	HitEffect->bAutoActivate = false;

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
	if (FVector::Dist(StartPos, GetActorLocation()) > OwnerCharacter->GetAttackRangeDepth())
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
	}
}

void AEUProjectile::OnEnemyCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSeep, const FHitResult& SweepResult)
{
	// Something is on the way of this object, then destroy this.
	StartDestroy();

	auto VictimCharacter = Cast<AEUCharacter>(OtherActor);
	if (VictimCharacter != nullptr)
	{
		// Apply damage to the victim.
		//

		auto OwnerCharacter = Cast<AEUCharacter>(GetOwner());
		EUCHECK(OwnerCharacter != nullptr);

		auto Controller = OwnerCharacter->GetController();
		EUCHECK(Controller != nullptr);

		FDamageEvent DamageEvent;
		VictimCharacter->TakeDamage(Damage, DamageEvent, Controller, this);
	}
}

void AEUProjectile::OnExplosionEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	HitEffect->Deactivate();
	Destroy();
}

void AEUProjectile::StartDestroy()
{
	if (bSetDestroy)
		return;

	SetActorEnableCollision(false);
	ProjectileMovement->Deactivate();

	FlyingEffect->Deactivate();
	HitEffect->Activate();
	HitEffect->OnSystemFinished.AddDynamic(this, &AEUProjectile::OnExplosionEffectFinished);

	bSetDestroy = true;
}