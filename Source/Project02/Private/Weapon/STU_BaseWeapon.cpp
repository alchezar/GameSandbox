// Copyright (C) 2023, IKinder

#include "Weapon/STU_BaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

ASTU_BaseWeapon::ASTU_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComponent");
	SetRootComponent(WeaponMesh);
}

void ASTU_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	CurrentAmmo = DefaultAmmo;
}

// Shoot
void ASTU_BaseWeapon::StartFire()
{
	MakeShot();
}

void ASTU_BaseWeapon::StopFire()
{}

void ASTU_BaseWeapon::Aiming()
{
	if (IsAmmoEmpty() || !GetCanAim()) return;
}

void ASTU_BaseWeapon::ToggleAim(const bool bAim)
{}

bool ASTU_BaseWeapon::GetCanAim() const
{
	return bCanAim;
}

bool ASTU_BaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

FRotator ASTU_BaseWeapon::GetWeaponSocketRotation() const
{
	return WeaponMesh->GetSocketRotation(SocketName);
}

void ASTU_BaseWeapon::SetCanAim(const bool bAim)
{
	bCanAim = bAim;
}

void ASTU_BaseWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(OUT TraceStart, OUT TraceEnd)) return;

	FHitResult Hit;
	MakeHit(OUT Hit, TraceStart, TraceEnd);

	if (Hit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), GetMuzzleSocketLocation(), Hit.ImpactPoint, FColor::Red, false, 3.f, 0, 3.f);
		DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 20.f, FColor::Red, false, 3.f);
		DrawDebugLine(GetWorld(), Hit.ImpactPoint, TraceEnd, FColor::Green, false, 3.f, 0, 3.f);

		MakeDamage(Hit);
	}
	else
	{
		DrawDebugLine(GetWorld(), GetMuzzleSocketLocation(), TraceEnd, FColor::Red, false, 3.f, 0, 3.f);
	}
	DecreaseAmmo();
}

bool ASTU_BaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(OUT TraceStart,OUT ViewRotation)) return false;
	TraceEnd = TraceStart + ViewRotation.Vector() * FVector(TraceMaxDistance);
	return true;
}

void ASTU_BaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	FCollisionQueryParams Params;
	// Params.AddIgnoredActor(GetOwner());
	Params.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(OUT HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
}

void ASTU_BaseWeapon::MakeDamage(const FHitResult& HitResult)
{ }

APlayerController* ASTU_BaseWeapon::GetPlayerController() const
{
	const auto* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

AController* ASTU_BaseWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

bool ASTU_BaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto OurCharacter = Cast<ACharacter>(GetOwner());
	if (!OurCharacter) return false;

	if (OurCharacter->IsPlayerControlled())
	{
		const APlayerController* Controller = GetPlayerController();
		if (!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleSocketLocation();
		ViewRotation = GetWeaponSocketRotation(); // WeaponMesh->GetSocketRotation(SocketName);
	}

	return true;
}

FVector ASTU_BaseWeapon::GetMuzzleSocketLocation() const
{
	return WeaponMesh->GetSocketLocation(SocketName);
}

// Ammo
FWeaponUIData ASTU_BaseWeapon::GetUIData() const
{
	return UIData;
}

FAmmoData ASTU_BaseWeapon::GetAmmoData() const
{
	return CurrentAmmo;
}

bool ASTU_BaseWeapon::TryToAddAmmo(const int32 Clips)
{
	if (CurrentAmmo.bInfinite || IsAmmoFull() || Clips <= 0) return false;

	if (IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(Clips, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + Clips;
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmo.Clips   = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}
	return true;
}

void ASTU_BaseWeapon::DecreaseAmmo()
{
	CurrentAmmo.Bullets--;
	// LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		OnClipEmpty.Broadcast(this);
	}
}

void ASTU_BaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.bInfinite && CurrentAmmo.Clips > 0)
	{
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ASTU_BaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

bool ASTU_BaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTU_BaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

void ASTU_BaseWeapon::LogAmmo() const
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " | ";
	AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : "Clips: " + FString::FromInt(CurrentAmmo.Clips);
}

// VFX
UNiagaraComponent* ASTU_BaseWeapon::SpawnMuzzleFX() const
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleFX,
		WeaponMesh,
		SocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);
}
