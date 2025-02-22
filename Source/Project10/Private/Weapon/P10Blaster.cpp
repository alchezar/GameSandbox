// Copyright (C) 2023, IKinder

#include "Weapon/P10Blaster.h"

#include "GameFramework/Actor.h"
#include "Weapon/P10Projectile.h"

AP10Blaster::AP10Blaster()
{
	PrimaryActorTick.bCanEverTick = false;

	if (!ProjectileClass)
	{
		ConstructorHelpers::FClassFinder<AP10Projectile> ProjectileClassFinder (TEXT("/Game/Project/PP10/Blueprint/Weapon/BP_Projectile_P10"));
		if (ProjectileClassFinder.Succeeded()) ProjectileClass = ProjectileClassFinder.Class;
	}
	
	BeamEffect = nullptr;
}

void AP10Blaster::BeginPlay()
{
	Super::BeginPlay();
	check(ProjectileClass)
}

void AP10Blaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Blaster::OneShot()
{
	const FVector MuzzleLocation = WeaponMeshComponent->GetSocketLocation(GetFirstSocketName());
	const FRotator MuzzleRotation = WeaponMeshComponent->GetSocketRotation(GetFirstSocketName());
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	Params.Owner = this;
	Params.Instigator = Cast<APawn>(GetOwner());
	
	GetWorld()->SpawnActor<AP10Projectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

	PlayMuzzleEffects();
	
	if (GetNextFireCount() >= 3 && FireMode == EP10FireMode::Brush)
	{
		StopFire();
	}
}
