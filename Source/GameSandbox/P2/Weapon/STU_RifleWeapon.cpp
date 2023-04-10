// Copyright (C) 2023, IKinder

#include "STU_RifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"

ASTU_RifleWeapon::ASTU_RifleWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_RifleWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_RifleWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimer, this, &ASTU_RifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ASTU_RifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimer);
}

void ASTU_RifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(OUT TraceStart, OUT TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult Hit;
	MakeHit(OUT Hit, TraceStart, TraceEnd);

	if (Hit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleSocketLocation(), Hit.ImpactPoint, FColor::Orange, false, 3.f, 0, 3.f);
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 20.f, FColor::Orange, false, 3.f);

		MakeDamage(Hit);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleSocketLocation(), TraceEnd, FColor::Green, false, 3.f, 0, 3.f);
	}
	DecreaseAmmo();
}

bool ASTU_RifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector  ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(OUT ViewLocation,OUT ViewRotation)) return false;
	TraceStart                   = ViewLocation;
	const auto    HalfRad        = FMath::DegreesToRadians(BulletSpread / 2);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd                     = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTU_RifleWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	Super::MakeHit(HitResult, TraceStart, TraceEnd);
}

void ASTU_RifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor) return;

	HitActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void ASTU_RifleWeapon::DecreaseAmmo()
{
	Super::DecreaseAmmo();
	
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
	}
}
