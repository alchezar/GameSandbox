// Copyright (C) 2023, IKinder

#include "STU_LauncherWeapon.h"
#include "STU_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	DrawProjectilePath();
}

void ASTU_LauncherWeapon::StopFire()
{
	MakeShot();
}

void ASTU_LauncherWeapon::MakeShot()
{
	const UWorld* World = GetWorld();
	if (!World) return;

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
}

void ASTU_LauncherWeapon::DrawProjectilePath()
{
	FVector  ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotation)) return;
	float ProjectileInitialSpeed = ProjectileClass.GetDefaultObject()->GetMovementComponent()->InitialSpeed;

	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation       = GetMuzzleSocketLocation();
	PredictParams.LaunchVelocity      = ViewRotation.Vector() * ProjectileInitialSpeed;
	PredictParams.MaxSimTime          = 5.f;
	PredictParams.bTraceWithCollision = true;
	PredictParams.bTraceWithChannel   = true;
	PredictParams.ActorsToIgnore      = {this, GetOwner()};
	PredictParams.TraceChannel        = ECollisionChannel::ECC_Visibility;
	PredictParams.DrawDebugType       = EDrawDebugTrace::None;

	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(GetWorld(), PredictParams, OUT PredictResult);

	// TODO: Add spline mesh instead of debug lines
	if (PredictResult.HitResult.bBlockingHit)
	{
		DrawDebugPoint(GetWorld(), PredictResult.HitResult.Location, 20.f, FColor::Red);
	}
	for (int32 i = 1; i < PredictResult.PathData.Num(); ++i)
	{
		FVector CurrentPointLocation  = PredictResult.PathData[i].Location;
		FVector PreviousPointLocation = PredictResult.PathData[i - 1].Location;

		DrawDebugLine(GetWorld(), PreviousPointLocation, CurrentPointLocation, FColor::Red, false, -1.f, 0, 3.f);
	}
}
