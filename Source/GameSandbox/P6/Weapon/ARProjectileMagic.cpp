// Copyright (C) 2023, IKinder

#include "ARProjectileMagic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "P6/Component/ARAttributesComponent.h"
#include "Particles/ParticleSystemComponent.h"

AARProjectileMagic::AARProjectileMagic()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARProjectileMagic::BeginPlay()
{
	Super::BeginPlay();
	MovementComp->bIsHomingProjectile = true;
	// MovementComp->HomingAccelerationMagnitude = 5000.f;
}

void AARProjectileMagic::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Target) return;
	const float CurrentDistance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	if (FMath::IsNearlyZero(CurrentDistance)) return;

	MovementComp->HomingAccelerationMagnitude = 100.f / CurrentDistance * 100000;
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Cyan, FString::Printf(TEXT("%f"), MovementComp->HomingAccelerationMagnitude));
}

void AARProjectileMagic::AddActorToIgnore(AActor* Actor)
{
	SphereComp->IgnoreActorWhenMoving(Actor, true);
}

void AARProjectileMagic::SetTarget(AActor* TheTarget)
{
	if (!TheTarget) return;

	Target = TheTarget;
	MovementComp->HomingTargetComponent = Target->FindComponentByClass<USceneComponent>();
}

void AARProjectileMagic::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// const FString DebugString = FString::Printf(TEXT("Hit location: %s"), *Hit.ImpactPoint.ToString());
	// DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugString, nullptr, FColor::Green, 5.f);
	if (Target)
	{
		Target->Destroy();
	}
	Super::OnProjectileHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AARProjectileMagic::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	const auto AttributeComp = Cast<UARAttributesComponent>(OtherActor->GetComponentByClass(UARAttributesComponent::StaticClass()));
	if (!AttributeComp) return;

	AttributeComp->TryChangeHealth(-Damage);
	Destroy();
}

void AARProjectileMagic::Explode()
{
	Super::Explode();
}
