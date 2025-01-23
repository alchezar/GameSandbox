// Copyright (C) 2023, IKinder

#include "Interact/Powerup/P10PickupActor.h"

#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Interact/Powerup/P10Powerup.h"

AP10PickupActor::AP10PickupActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	// SetReplicates(true);
	bReplicates = true;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(100.f);
	SphereComponent->SetGenerateOverlapEvents(true);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	DecalComponent->DecalSize = FVector(64.f, 100.f, 100.f);
}

void AP10PickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Respawn();
	}
}

void AP10PickupActor::Respawn()
{
	check(PowerupClass)
	
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerupInstance = GetWorld()->SpawnActor<AP10Powerup>(PowerupClass, GetActorTransform(), Params);
}

void AP10PickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!OtherActor || !PowerupInstance) return;

	if (HasAuthority())
	{
		PowerupInstance->ActivatePowerup(OtherActor);
		PowerupInstance = nullptr;
		GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &ThisClass::Respawn, Cooldown);
	}
}
