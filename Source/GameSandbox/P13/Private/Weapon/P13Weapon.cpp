// Copyright © 2024, IKinder

#include "P13/Public/Weapon/P13Weapon.h"

#include "Components/ArrowComponent.h"
#include "P13/Public/Weapon/P13ProjectileDefault.h"

AP13Weapon::AP13Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneComponent);

	MeshWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	MeshWeapon->SetupAttachment(RootComponent);
	MeshWeapon->SetGenerateOverlapEvents(false);
	MeshWeapon->SetCollisionProfileName("NoCollision");

	ShootLocation = CreateDefaultSubobject<UArrowComponent>("ShootLocationArrowComponent");
	ShootLocation->SetupAttachment(RootComponent);
}

void AP13Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AP13Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13Weapon::SetFireState(bool bFiring)
{
	FTimerManager* TimerManager = &GetWorld()->GetTimerManager();
	if (!bFiring && TimerManager->IsTimerActive(FireTimer))
	{
		TimerManager->ClearTimer(FireTimer);
		return;
	}
	
	if (!CheckWeaponCanFire())
	{
		return;
	}
	const float SecondsPerShot = 60.f / WeaponSettings.RateOfFire;
	TimerManager->SetTimer(FireTimer, this, &ThisClass::Fire, SecondsPerShot, true, 0.f);
	
}

void AP13Weapon::Fire()
{
	if (const TSubclassOf<AP13ProjectileDefault> ProjectileClass = WeaponSettings.ProjectileSettings.Class)
	{
		SpawnProjectile(ProjectileClass);
		return;
	}
	// TODO: line trace shot
}

bool AP13Weapon::CheckWeaponCanFire()
{
	return true;
}

void AP13Weapon::SpawnProjectile(TSubclassOf<AP13ProjectileDefault> ProjectileClass)
{
	const FP13ProjectileInfo Projectile = WeaponSettings.ProjectileSettings;
	if (!Projectile.Class)
	{
		return;
	}
	check(ShootLocation)

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	auto* Bullet = GetWorld()->SpawnActor<AP13ProjectileDefault>(Projectile.Class, ShootLocation->GetComponentTransform(), SpawnParams);
	if (!Bullet)
	{
		return;
	}
	Bullet->InitialLifeSpan = 20.f;
	// Bullet->GetBulletMovement()->InitialSpeed = 2000.f;
}
