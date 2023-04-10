// Copyright (C) 2023, IKinder

#include "STU_BaseWeapon.h"
#include "DrawDebugHelpers.h"
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

void ASTU_BaseWeapon::StartFire()
{
	MakeShot();
}

void ASTU_BaseWeapon::StopFire()
{ }

void ASTU_BaseWeapon::Aiming()
{
	if (IsAmmoEmpty() || !bCanAim) return;
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

APlayerController* ASTU_BaseWeapon::GetPlayerController() const
{
	const auto* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

FVector ASTU_BaseWeapon::GetMuzzleSocketLocation() const
{
	return WeaponMesh->GetSocketLocation(SocketName);
}

bool ASTU_BaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(OUT TraceStart,OUT ViewRotation)) return false;
	TraceEnd = TraceStart + ViewRotation.Vector() * TraceMaxDistance;
	return true;
}

bool ASTU_BaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const APlayerController* Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

void ASTU_BaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(OUT HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
}

void ASTU_BaseWeapon::MakeDamage(const FHitResult& HitResult)
{ }

#pragma region Ammo

void ASTU_BaseWeapon::DecreaseAmmo()
{
	CurrentAmmo.Bullets--;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		OnClipEmpty.Broadcast();
	}
}

void ASTU_BaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.bInfinite && CurrentAmmo.Clips > 0)
	{
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Emerald, FString::Printf(TEXT("Changed clip")));
}

bool ASTU_BaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ASTU_BaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTU_BaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTU_BaseWeapon::LogAmmo() const
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " | ";
	AmmoInfo += CurrentAmmo.bInfinite ? "Infinite" : "Clips: " + FString::FromInt(CurrentAmmo.Clips);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Turquoise, FString::Printf(TEXT("%s"), *AmmoInfo));
}

#pragma endregion // Ammo
