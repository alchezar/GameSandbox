// Copyright © 2024, IKinder

#include "P13/Public/Weapon/P13Weapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
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

void AP13Weapon::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine) GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, FString::Printf(TEXT("%f"), DispersionAngle));
}

FVector AP13Weapon::GetShootLocation() const
{
	return ShootLocation->GetComponentLocation();
}

int32 AP13Weapon::GetWeaponRound()
{
	return WeaponCurrentSettings.Round;
}

void AP13Weapon::WeaponInit(FP13WeaponInfo* WeaponInfo, const EP13MovementState NewState, const USkeletalMeshComponent* Mesh)
{
	WeaponSettings = WeaponInfo;
	FinishReload();
	UpdateWeaponState(NewState);
}

void AP13Weapon::UpdateWeaponState(const EP13MovementState NewState)
{
	if (NewState == EP13MovementState::Aim)
	{
		CurrentDispersion = WeaponSettings->WeaponDispersion.Aim;
	}
	else if (NewState == EP13MovementState::Walk)
	{
		CurrentDispersion = WeaponSettings->WeaponDispersion.Walk;
	}
	else if (NewState == EP13MovementState::Run)
	{
		CurrentDispersion = WeaponSettings->WeaponDispersion.Run;
	}

	DispersionAngle = CurrentDispersion.Min;
	// UpdateDispersion();
}

void AP13Weapon::SetTargetLocation(const FVector& TargetLocation)
{
	ShotTargetLocation = TargetLocation;
}

void AP13Weapon::SetFireState(const bool bFiring)
{
	if (!bFiring)
	{
		/* Stop fire timer when button released. */
		GetWorld()->GetTimerManager().ClearTimer(FireTimer);
		/* Start reducing bullet disperse after stop fire. */
		GetWorld()->GetTimerManager().SetTimer(DisperseTimer, this, &ThisClass::DisperseReducing, GetWorld()->GetDeltaSeconds(), true, 0.f);
		return;
	}

	/* Check  if there are any reasons why the weapon itself cannot fire. */
	if (!CheckWeaponCanFire())
	{
		return;
	}

	/* Stop reducing bullet disperse. */
	GetWorld()->GetTimerManager().ClearTimer(DisperseTimer);
	/* Start fire. */
	const float SecondsPerShot = 60.f / WeaponSettings->RateOfFire;
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::Fire, SecondsPerShot, true, 0.f);
	LastShotTime = GetWorld()->GetTimeSeconds();
}

void AP13Weapon::TryReloadForce()
{
	if (GetWeaponRound() >= WeaponSettings->MaxRound)
	{
		return;
	}
	InitReload();
}

bool AP13Weapon::CheckWeaponCanFire()
{
	bool bResult = true;

	const double CurrentShotTime = GetWorld()->GetTimeSeconds();
	const double TimeFromLastShot = CurrentShotTime - LastShotTime;
	const double MinTimeBetweenShots = 60.f / WeaponSettings->RateOfFire;
	bResult = bResult && (MinTimeBetweenShots < TimeFromLastShot);
	bResult = bResult && (!GetWorld()->GetTimerManager().IsTimerActive(FireTimer));
	bResult = bResult && (!bReloading);

	return bResult;
}

void AP13Weapon::Fire()
{
	if (WeaponCurrentSettings.Round-- <= 0.f)
	{
		SetFireState(false);
		InitReload();
		return;
	}

	for (int32 Index = 0; Index < WeaponSettings->ProjectilesPerShot; ++Index)
	{
		if (WeaponSettings->ProjectileSettings.Class)
		{
			SpawnProjectile();
		}
		else
		{
			// TODO: Line trace shot.
		}

		OnWeaponFire.Broadcast(WeaponSettings->CharFireAnim);
		SpawnEffectsAtLocation(WeaponSettings->FireSound, WeaponSettings->FireEffect, ShootLocation->GetComponentLocation());
		PlayAnimMontage(WeaponSettings->WeaponFireAnim);
		UpdateDispersion();
	}
}

void AP13Weapon::SpawnProjectile() const
{
	const TSubclassOf<AP13ProjectileDefault> ProjectileClass = WeaponSettings->ProjectileSettings.Class;
	if (!ProjectileClass)
	{
		return;
	}
	check(ShootLocation)
	const FTransform ShotTransform = FTransform(GetFinalDirection().Rotation(), ShootLocation->GetComponentLocation());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	auto* Bullet = GetWorld()->SpawnActorDeferred<AP13ProjectileDefault>(ProjectileClass, ShotTransform, GetOwner(), GetInstigator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (!Bullet)
	{
		return;
	}
	Bullet->InitBullet(20.f, WeaponSettings->ProjectileSettings.InitSpeed);
	Bullet->FinishSpawning(ShotTransform);
}

FVector AP13Weapon::GetFinalDirection() const
{
	//  /* By using matrix. */ 
	// const FMatrix ShootMatrix = FMatrix(ShootDirection, FVector(0.f, 1.f, 0.f), FVector(0.f, 0.f, 1.f), FVector::ZeroVector);
	// return ShootMatrix.Rotator();

	const float BarrelHeight = ShootLocation->GetComponentLocation().Z;
	const float PawnHeight = GetInstigator()->GetActorLocation().Z;
	const float HalfCapsuleHeight = GetInstigator<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	constexpr float GravityCompensation = 10.f;

	const float Displacement = BarrelHeight - PawnHeight + HalfCapsuleHeight - GravityCompensation;

	const FVector RaisedTargetLocation = ShotTargetLocation + FVector::UpVector * Displacement;
	const FVector ShootDirection = (RaisedTargetLocation - ShootLocation->GetComponentLocation()).GetSafeNormal();
	const FVector RandShootDirection = FMath::VRandCone(ShootDirection, FMath::DegreesToRadians(DispersionAngle / 2.f));

	return RandShootDirection;
}

void AP13Weapon::UpdateDispersion(const bool bRest)
{
	const float DispersionTick = bRest ? -CurrentDispersion.Reduction : CurrentDispersion.Recoil;
	DispersionAngle = FMath::Clamp(DispersionAngle + DispersionTick, CurrentDispersion.Min, CurrentDispersion.Max);
}

void AP13Weapon::DisperseReducing()
{
	if (DispersionAngle <= CurrentDispersion.Min)
	{
		GetWorld()->GetTimerManager().ClearTimer(DisperseTimer);
		return;
	}
	UpdateDispersion(true);
}

void AP13Weapon::InitReload()
{
	bReloading = true;

	FTimerHandle ReloadTimer;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ThisClass::FinishReload, WeaponSettings->ReloadTime);

	OnWeaponReload.Broadcast(true, WeaponSettings->CharReloadAnim);
	PlayAnimMontage(WeaponSettings->WeaponReloadAnim);	
}

void AP13Weapon::FinishReload()
{
	bReloading = false;
	WeaponCurrentSettings.Round = WeaponSettings->MaxRound;
	OnWeaponReload.Broadcast(false, nullptr);
	StopAnimMontage(WeaponSettings->WeaponReloadAnim);
}

void AP13Weapon::SpawnEffectsAtLocation(USoundBase* SoundBase, UNiagaraSystem* NiagaraSystem, const FVector& Location) const
{
	UGameplayStatics::SpawnSoundAtLocation(this, SoundBase, Location);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraSystem, Location);
}

float AP13Weapon::PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate, const FName StartSectionName)
{
	UAnimInstance* WeaponAnimInstance = MeshWeapon->GetAnimInstance();
	if (!WeaponAnimInstance)
	{
		return 0.f;
	}
	const float Duration = WeaponAnimInstance->Montage_Play(AnimMontage, InPlayRate);
	if (Duration <= 0)
	{
		return 0.f;
	}
	if (StartSectionName != NAME_None)
	{
		WeaponAnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
	}
	return Duration;
}

void AP13Weapon::StopAnimMontage(const UAnimMontage* AnimMontage)
{
	UAnimInstance* WeaponAnimInstance = MeshWeapon->GetAnimInstance();
	if (!WeaponAnimInstance)
	{
		return;
	}
	const UAnimMontage* MontageToStop = AnimMontage ? AnimMontage : WeaponAnimInstance->GetCurrentActiveMontage();
	if (!MontageToStop || WeaponAnimInstance->Montage_GetIsStopped(MontageToStop))
	{
		return;
	}
	WeaponAnimInstance->Montage_Stop(MontageToStop->BlendOut.GetBlendTime(), MontageToStop);
}
