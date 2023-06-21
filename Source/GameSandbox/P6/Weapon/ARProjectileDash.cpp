// Copyright (C) 2023, IKinder

#include "ARProjectileDash.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AARProjectileDash::AARProjectileDash()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARProjectileDash::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DetonateTimer;
	GetWorld()->GetTimerManager().SetTimer(DetonateTimer, this, &ThisClass::Explode, DetonateDelay);
}

void AARProjectileDash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARProjectileDash::Explode()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact, GetActorLocation(), GetActorRotation());
	EffectComp->DeactivateSystem();
	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TeleportHandle;
	GetWorld()->GetTimerManager().SetTimer(TeleportHandle, this, &ThisClass::TeleportInstigator, TeleportDelay);
	
}

void AARProjectileDash::TeleportInstigator()
{
	AActor* ActorToTeleport = GetInstigator();
	if (!ActorToTeleport) return;

	ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation());
}

