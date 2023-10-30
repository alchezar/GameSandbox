// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"

#include "GameSandbox.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "P12/Public/Actor/Projectile/P12Projectile.h"
#include "P12/Public/Util/P12Library.h"

UP12WeaponBarrelComponent::UP12WeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12WeaponBarrelComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP12WeaponBarrelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP12WeaponBarrelComponent::Shot(const FVector& ShotStart, const FVector& ShotDirection, AController* Instigator, const float SpreadAngle)
{
	for (int i = 0; i < BulletsPerShot; ++i)
	{
		const float HalfAngleRad = FMath::DegreesToRadians(SpreadAngle / 2.f);
		const FVector SpreadDirection = FMath::VRandCone(ShotDirection, HalfAngleRad);
		const FVector ShotEnd = ShotStart + FiringRange * SpreadDirection;

		if (HitRegistrationType == EP12HitRegistrationType::HitScan)
		{
			FHitResult ShotHitResult;
			ShotHitResult = HitScan(ShotStart, ShotEnd, ShotDirection);
			UP12Library::DrawDebugLineTrace(GetWorld(), ShotHitResult, UP12Library::GetCanDrawDebugFire(), false);
			
			/* Trace tale Niagara. */
			const FVector MuzzleLocation = GetComponentLocation();
			if (TraceNiagara)
			{
				const FVector NiagaraLocation = ShotHitResult.bBlockingHit ? ShotHitResult.ImpactPoint : ShotHitResult.TraceEnd;
				UNiagaraComponent* TraceNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceNiagara, MuzzleLocation);
				if (!TraceNiagaraComp)
				{
					continue;
				}
				TraceNiagaraComp->SetVectorParameter("TraceEnd", NiagaraLocation);
			}
		}
		if (HitRegistrationType == EP12HitRegistrationType::Projectile)
		{
			ProjectileLaunch(ShotStart, ShotDirection);
		}
	}
}

FHitResult UP12WeaponBarrelComponent::HitScan(const FVector& Start, const FVector& End, const FVector& Direction)
{
	FHitResult HitScanResult;
	GetWorld()->LineTraceSingleByChannel(HitScanResult, Start, End, ECC_BULLET);
	if (HitScanResult.bBlockingHit)
	{
		/* Take Damage. */
		AActor* HitActor = HitScanResult.GetActor();
		if (!HitActor)
		{
			return HitScanResult;
		}

		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = HitScanResult;
		DamageEvent.ShotDirection = Direction;
		DamageEvent.DamageTypeClass = DamageTypeClass;
		HitActor->TakeDamage(DamageAmount, DamageEvent, GetOwningController(), GetOwner());

		/* Draw decal. */
		if (UDecalComponent* BulletHoleDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalInfo.Decal, DecalInfo.Size, HitScanResult.ImpactPoint, HitScanResult.ImpactNormal.ToOrientationRotator()))
		{
			BulletHoleDecal->SetFadeOut(DecalInfo.Lifetime, DecalInfo.FadeOutTime);
			BulletHoleDecal->SetFadeScreenSize(DecalInfo.FadeScreenSize);
		}
	}
	return HitScanResult;
}

void UP12WeaponBarrelComponent::ProjectileLaunch(const FVector& Start, const FVector& Direction)
{
	if (AP12Projectile* Projectile = GetWorld()->SpawnActor<AP12Projectile>(ProjectileClass, Start, Direction.ToOrientationRotator()))
	{
		Projectile->SetOwner(GetOwningPawn());
		Projectile->LaunchProjectile(Direction.GetSafeNormal(), GetOwner());
	}
}

APawn* UP12WeaponBarrelComponent::GetOwningPawn() const
{
	AActor* Causer = GetOwner();
	if (!Causer)
	{
		return nullptr;
	}
	APawn* CauserPawn = Cast<APawn>(Causer);
	if (!CauserPawn)
	{
		CauserPawn = Cast<APawn>(Causer->GetOwner());
	}
	return CauserPawn;
}

AController* UP12WeaponBarrelComponent::GetOwningController() const
{
	if (!GetOwningPawn())
	{
		return nullptr;
	}
	return GetOwningPawn()->Controller;
}
