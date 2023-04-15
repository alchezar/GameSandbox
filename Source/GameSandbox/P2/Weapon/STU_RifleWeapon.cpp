// Copyright (C) 2023, IKinder

#include "STU_RifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "GameSandbox/P2/Weapon/Component/STU_WeaponFXComponent.h"

ASTU_RifleWeapon::ASTU_RifleWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponFXComponent = CreateDefaultSubobject<USTU_WeaponFXComponent>("WeaponFXComponent");
}

void ASTU_RifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ASTU_RifleWeapon::StartFire()
{
	GetWorldTimerManager().SetTimer(ShotTimer, this, &ASTU_RifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
	InitMuzzleFX();
}

void ASTU_RifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimer);
	SetMuzzleFXVisibility(false);
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

	FVector TraceFXEnd = TraceEnd;

	FHitResult Hit;
	MakeHit(OUT Hit, TraceStart, TraceEnd);

	if (Hit.bBlockingHit)
	{
		TraceFXEnd = Hit.ImpactPoint;
		WeaponFXComponent->PlayImpactFX(Hit);
		MakeDamage(Hit);
	}
	DecreaseAmmo();
	SpawnTraceFX(GetMuzzleSocketLocation(), TraceFXEnd);
}

bool ASTU_RifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
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

	//TODO: head bone name hardcode isn`t great idea 😅 it will be better to make an array
	const bool  bHeadshot     = HitResult.BoneName == FName("head");
	const float CurrentDamage = bHeadshot ? DamageAmount * HeadshotMultiplier : DamageAmount;

	HitActor->TakeDamage(CurrentDamage, FDamageEvent(), GetPlayerController(), this);
}

void ASTU_RifleWeapon::DecreaseAmmo()
{
	Super::DecreaseAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
	}
}

void ASTU_RifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ASTU_RifleWeapon::SetMuzzleFXVisibility(const bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible);
	}
}

void ASTU_RifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetVariableVec3(TraceTargetName, TraceEnd);
	}
}
