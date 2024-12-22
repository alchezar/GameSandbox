// Copyright (C) 2023, IKinder

#include "STU_BlasterWeapon.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile/STU_ProjectileBullet.h"
#include "Sound/SoundCue.h"

ASTU_BlasterWeapon::ASTU_BlasterWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASTU_BlasterWeapon::BeginPlay()
{
	Super::BeginPlay();
	DefaultBulletSpread = BulletSpread;
}

void ASTU_BlasterWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimer, this, &ASTU_BlasterWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
	InitMuzzleFX();
}

void ASTU_BlasterWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimer);
	SetMuzzleFXVisibility(false);
}

void ASTU_BlasterWeapon::ToggleAim(const bool bAim)
{
	const APlayerController* Controller = Cast<APlayerController>(GetController());
	if (!Controller || !Controller->PlayerCameraManager) return;

	if (bAim)
	{
		DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}
	Controller->PlayerCameraManager->SetFOV(bAim ? ZoomFOV : DefaultCameraFOV);
	BulletSpread = bAim ? BulletSpread / 2 : DefaultBulletSpread;
}

void ASTU_BlasterWeapon::MakeShot()
{
	FVector TraceStart, TraceEnd;
	if (IsAmmoEmpty() || !GetTraceData(OUT TraceStart, OUT TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(OUT HitResult, TraceStart, TraceEnd);

	const FVector EndPoint  = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleSocketLocation()).GetSafeNormal();

	auto SpawnTransform  = FTransform(FRotator::ZeroRotator, GetMuzzleSocketLocation());
	auto SpawnProjectile = GetWorld()->SpawnActorDeferred<ASTU_ProjectileBullet>(BulletClass, SpawnTransform);
	if (SpawnProjectile)
	{
		SpawnProjectile->SetShotDirection(Direction);
		SpawnProjectile->SetOwner(this);
		SpawnProjectile->FinishSpawning(SpawnTransform);
	}
	DecreaseAmmo();
	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, SocketName);
}

bool ASTU_BlasterWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(OUT ViewLocation,OUT ViewRotation)) return false;

	TraceStart          = ViewLocation;
	const float HalfRad = FMath::DegreesToRadians(BulletSpread / 2);
	ShootDirection      = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd            = TraceStart + ShootDirection * FVector(TraceMaxDistance);
	return true;
}

void ASTU_BlasterWeapon::DecreaseAmmo()
{
	Super::DecreaseAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
	}
}

void ASTU_BlasterWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTU_BlasterWeapon::SetMuzzleFXVisibility(const bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible);
	}
}

FVector ASTU_BlasterWeapon::GetShootDirection() const
{
	return ShootDirection;
}
