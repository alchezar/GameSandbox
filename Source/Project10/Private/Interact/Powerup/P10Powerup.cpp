// Copyright (C) 2023, IKinder

#include "Interact/Powerup/P10Powerup.h"

AP10Powerup::AP10Powerup()
{
	PrimaryActorTick.bCanEverTick = true;
	// SetReplicates(true);
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComponent->SetupAttachment(RootComponent);
}

void AP10Powerup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MeshComponent->AddRelativeRotation(FRotator(0.f, 360.f * DeltaSeconds / MeshFullRotationTime, 0.f));
}

void AP10Powerup::ActivatePowerup(AActor* Target)
{
	/* This method was called on the server. */
	
	Multicast_OnActivated(Target);

	if (Interval > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(PowerupTimer, this, &ThisClass::OnTickPowerup, Interval, true);
	}
	else /* Interval <= 0 */
	{
		OnTickPowerup();
	}
}

void AP10Powerup::Multicast_OnActivated_Implementation(AActor* Target)
{
	OnActivated(Target);
}

void AP10Powerup::OnActivated(AActor* Target)
{
	MeshComponent->SetVisibility(false);
}

void AP10Powerup::OnTickPowerup()
{
	if (--TotalTicks > 0) return;
	Multicast_OnExpired();
}

void AP10Powerup::Multicast_OnExpired_Implementation()
{
	OnExpired();
}
void AP10Powerup::OnExpired()
{
	GetWorld()->GetTimerManager().ClearTimer(PowerupTimer);
	Destroy();
}
