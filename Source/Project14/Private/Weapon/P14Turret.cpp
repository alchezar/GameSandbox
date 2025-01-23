// Copyright © 2025, Ivan Kinder

#include "Weapon/P14Turret.h"

#include "Weapon/P14Projectile.h"

AP14Turret::AP14Turret()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	check(SceneRoot)
	SetRootComponent(SceneRoot.Get());

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	check(TurretMesh)
	TurretMesh->SetupAttachment(SceneRoot.Get());
}

void AP14Turret::BeginPlay()
{
	Super::BeginPlay();

	check(AmmoCount > 0)
	check(FireFrequency > 0.f)

	const float FirstDelay = FireFrequency;
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::OnFire, FireFrequency, true, FirstDelay);
}

void AP14Turret::OnFire()
{
	if (--AmmoCount <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FTransform SocketTransform = TurretMesh->GetSocketTransform("Muzzle");
	AP14Projectile*  Projectile      = World->SpawnActorDeferred<AP14Projectile>(ProjectileClass, SocketTransform);
	if (!Projectile)
	{
		return;
	}

	Projectile->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
	Projectile->FinishSpawning(SocketTransform);
}
