// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"

#include "GameSandbox.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "P12/Public/Actor/Projectile/P12Projectile.h"
#include "P12/Public/Util/P12Library.h"

UP12WeaponBarrelComponent::UP12WeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UP12WeaponBarrelComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()->HasAuthority() || !ProjectileClass)
	{
		return;
	}

	ProjectilePool.Reserve(ProjectilePoolSize);
	for (int i = 0; i < ProjectilePoolSize; ++i)
	{
		// AP12Projectile* Projectile = GetWorld()->SpawnActor<AP12Projectile>(ProjectileClass, ProjectilePoolLocation, FRotator::ZeroRotator);
		AP12Projectile* Projectile = GetWorld()->SpawnActor<AP12Projectile>(ProjectileClass);
		check(Projectile)

		Projectile->SetOwner(GetOwningPawn());
		Projectile->ToggleActive(false, ProjectilePoolLocation);
		ProjectilePool.Add(Projectile);
	}
}

void UP12WeaponBarrelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP12WeaponBarrelComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams RepParams;
	RepParams.Condition = COND_SimulatedOnly;
	RepParams.RepNotifyCondition = REPNOTIFY_Always;
	DOREPLIFETIME_WITH_PARAMS(ThisClass, LastShotsInfo, RepParams);

	DOREPLIFETIME(ThisClass, ProjectilePool);
	DOREPLIFETIME(ThisClass, CurrentProjectileIndex);
}

void UP12WeaponBarrelComponent::Shot(const FVector& ShotStart, const FVector& ShotDirection, const float SpreadAngle)
{
	TArray<FP12ShotInfo> ShotInfos;
	for (int i = 0; i < BulletsPerShot; ++i)
	{
		const float HalfAngleRad = FMath::DegreesToRadians(SpreadAngle / 2.f);
		const FVector SpreadDirection = FMath::VRandCone(ShotDirection, HalfAngleRad);

		// ShotInfos.Add({ShotStart, SpreadDirection});
		ShotInfos.Emplace(ShotStart, SpreadDirection);
	}

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_Shot(ShotInfos);
	}
	ShotInternal(ShotInfos);
}

void UP12WeaponBarrelComponent::DrawNiagaraTale(const FVector& EndPoint)
{
	const FVector MuzzleLocation = GetComponentLocation();
	if (!TraceNiagara)
	{
		return;
	}
	UNiagaraComponent* TraceNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceNiagara, MuzzleLocation);
	if (!TraceNiagaraComp)
	{
		return;
	}
	TraceNiagaraComp->SetVectorParameter("TraceEnd", EndPoint);
}

void UP12WeaponBarrelComponent::HitScan(const FVector& Start, const FVector& End, const FVector& Direction, FHitResult& Out_HitResult)
{
	GetWorld()->LineTraceSingleByChannel(Out_HitResult, Start, End, ECC_BULLET);
	if (Out_HitResult.bBlockingHit)
	{
		ProcessHit(Out_HitResult, Direction);
	}
}

void UP12WeaponBarrelComponent::ProjectileLaunch(const FVector& Start, const FVector& Direction)
{
	// AP12Projectile* Projectile = GetWorld()->SpawnActor<AP12Projectile>(ProjectileClass, Start, Direction.ToOrientationRotator());
	AP12Projectile* Projectile = ProjectilePool[CurrentProjectileIndex];
	check(Projectile)
	Projectile->OnProjectileHit.AddUObject(this, &ThisClass::ProcessProjectileHit);
	Projectile->ToggleActive(true, Start, Direction, GetOwner());

	if (++CurrentProjectileIndex == ProjectilePool.Num())
	{
		CurrentProjectileIndex = 0;
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

void UP12WeaponBarrelComponent::ProcessHit(const FHitResult& HitResult, const FVector& Direction)
{
	/* Take Damage. */
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
	{
		return;
	}
	if (GetOwner()->HasAuthority())
	{
		FPointDamageEvent DamageEvent;
		DamageEvent.HitInfo = HitResult;
		DamageEvent.ShotDirection = Direction;
		DamageEvent.DamageTypeClass = DamageTypeClass;
		HitActor->TakeDamage(DamageAmount, DamageEvent, GetOwningController(), GetOwner());
	}

	/* Draw decal. */
	if (UDecalComponent* BulletHoleDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalInfo.Decal, DecalInfo.Size, HitResult.ImpactPoint, HitResult.ImpactNormal.ToOrientationRotator()))
	{
		BulletHoleDecal->SetFadeOut(DecalInfo.Lifetime, DecalInfo.FadeOutTime);
		BulletHoleDecal->SetFadeScreenSize(DecalInfo.FadeScreenSize);
	}
}

void UP12WeaponBarrelComponent::ShotInternal(const TArray<FP12ShotInfo>& ShotsInfo)
{
	if (GetOwner()->HasAuthority())
	{
		LastShotsInfo = ShotsInfo;
	}
	
	for (const FP12ShotInfo ShotInfo : ShotsInfo)
	{
		const FVector ShotStart = ShotInfo.GetLocation();
		const FVector ShotDirection = ShotInfo.GetDirection();
		const FVector ShotEnd = ShotStart + FiringRange * ShotDirection;

		if (HitRegistrationType == EP12HitRegistrationType::HitScan)
		{
			FHitResult ShotHitResult;
			HitScan(ShotStart, ShotEnd, ShotDirection, ShotHitResult);
			
			const FVector EndPoint = ShotHitResult.bBlockingHit ? ShotHitResult.ImpactPoint : ShotHitResult.TraceEnd; 
			DrawNiagaraTale(EndPoint);
			
			UP12Library::DrawDebugLineTrace(GetWorld(), ShotHitResult, UP12Library::GetCanDrawDebugFire(), false);
		}
		if (HitRegistrationType == EP12HitRegistrationType::Projectile)
		{
			ProjectileLaunch(ShotStart, ShotDirection);
		}
	}
}


void UP12WeaponBarrelComponent::Server_Shot_Implementation(const TArray<FP12ShotInfo>& ShotsInfo)
{
	ShotInternal(ShotsInfo);
}

void UP12WeaponBarrelComponent::OnRep_LastShotsInfo()
{
	ShotInternal(LastShotsInfo);
}

void UP12WeaponBarrelComponent::ProcessProjectileHit(const FHitResult& HitResult, const FVector& Direction, AP12Projectile* Projectile)
{
	/* Deactivate projectile */
	Projectile->OnProjectileHit.RemoveAll(this);
	Projectile->ToggleActive(false, ProjectilePoolLocation);
	
	ProcessHit(HitResult, Direction);	
}
