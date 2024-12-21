// Copyright (C) 2023, IKinder

#include "Weapon/STU_LauncherWeapon.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon/STU_Projectile.h"

ASTU_LauncherWeapon::ASTU_LauncherWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_LauncherWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_LauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTU_LauncherWeapon::StopFire()
{}

void ASTU_LauncherWeapon::Aiming()
{
	if (IsAmmoEmpty() || !GetCanAim()) return;

	DrawProjectilePath();
}

void ASTU_LauncherWeapon::ChangeClip()
{
	Super::ChangeClip();
}

void ASTU_LauncherWeapon::MakeShot()
{
	if (!GetWorld()) return;
	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetMuzzleSocketLocation());
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(OUT TraceStart, OUT TraceEnd)) return;

	FHitResult Hit;
	MakeHit(OUT Hit, TraceStart, TraceEnd);

	const FVector EndPoint  = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleSocketLocation()).GetSafeNormal();

	FTransform       SpawnTransform  = FTransform(FRotator::ZeroRotator, GetMuzzleSocketLocation());
	ASTU_Projectile* SpawnProjectile = GetWorld()->SpawnActorDeferred<ASTU_Projectile>(ProjectileClass, SpawnTransform);

	if (SpawnProjectile)
	{
		SpawnProjectile->SetShotDirection(Direction);
		SpawnProjectile->SetOwner(GetOwner());
		SpawnProjectile->FinishSpawning(SpawnTransform);
	}
	DecreaseAmmo();
	SpawnMuzzleFX();

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, SocketName);
}

void ASTU_LauncherWeapon::DrawProjectilePath()
{
	FVector  ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotation)) return;

	ASTU_Projectile* Projectile             = ProjectileClass.GetDefaultObject();
	float            ProjectileInitialSpeed = Projectile->GetMovementComponent()->InitialSpeed;

	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation       = GetMuzzleSocketLocation();
	PredictParams.LaunchVelocity      = ViewRotation.Vector() * ProjectileInitialSpeed;
	PredictParams.MaxSimTime          = 5.f;
	PredictParams.bTraceWithCollision = true;
	PredictParams.bTraceWithChannel   = true;
	PredictParams.ActorsToIgnore      = {this, GetOwner(), Projectile};
	PredictParams.TraceChannel        = ECollisionChannel::ECC_Visibility;
	PredictParams.DrawDebugType       = EDrawDebugTrace::None;
	PredictParams.OverrideGravityZ *= Projectile->GetMovementComponent()->ProjectileGravityScale;

	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, OUT PredictResult);

	// TODO: Add spline mesh instead of debug lines
	if (PredictResult.HitResult.bBlockingHit)
	{
		DrawDebugPoint(GetWorld(), PredictResult.HitResult.Location, 20.f, FColor::Cyan);
	}
	for (int32 i = 1; i < PredictResult.PathData.Num(); ++i)
	{
		FVector CurrentPointLocation  = PredictResult.PathData[i].Location;
		FVector PreviousPointLocation = PredictResult.PathData[i - 1].Location;

		DrawDebugLine(GetWorld(), PreviousPointLocation, CurrentPointLocation, FColor::Cyan, false, -1.f, 0, 3.f);
	}
}
