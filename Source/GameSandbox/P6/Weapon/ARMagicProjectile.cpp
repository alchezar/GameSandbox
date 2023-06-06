// Copyright (C) 2023, IKinder

#include "ARMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "Particles/ParticleSystemComponent.h"

AARMagicProjectile::AARMagicProjectile()
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
}

void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	MovementComp->bIsHomingProjectile = true;
	MovementComp->HomingAccelerationMagnitude = 5000.f;
}

void AARMagicProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileBeginOverlap);
}

void AARMagicProjectile::AddActorToIgnore(AActor* Actor)
{
	SphereComp->IgnoreActorWhenMoving(Actor, true);
}

void AARMagicProjectile::SetTarget(AActor* TheTarget)
{
	if (!TheTarget) return;
	
	Target = TheTarget;
	MovementComp->HomingTargetComponent = Target->FindComponentByClass<USceneComponent>();
}

void AARMagicProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FString DebugString = FString::Printf(TEXT("Hit location: %s"), *Hit.ImpactPoint.ToString());
	// DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugString, nullptr, FColor::Green, 5.f);
	if(Target)
	{
		Target->Destroy();
	}
}

void AARMagicProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	
	const auto AttributeComp = Cast<UARAttributesComponent>(OtherActor->GetComponentByClass(UARAttributesComponent::StaticClass()));
	if (!AttributeComp) return;

	AttributeComp->TryChangeHealth(-Damage);
	Destroy();
}