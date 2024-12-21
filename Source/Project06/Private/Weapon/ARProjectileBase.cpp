// Copyright (C) 2023, IKinder

#include "Weapon/ARProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AARProjectileBase::AARProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName("Projectile");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.f;

	// SetReplicates(true);
	bReplicates = true;
}

void AARProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

void AARProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileBeginOverlap);
}

void AARProjectileBase::AddActorToIgnore(AActor* Actor)
{
	SphereComp->IgnoreActorWhenMoving(Actor, true);
}

void AARProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AARProjectileBase::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AARProjectileBase::Explode()
{
	if (!IsValid(this)) return;
	
	if (Impact)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact, GetActorLocation(), GetActorRotation());
	}
	Destroy();
}

