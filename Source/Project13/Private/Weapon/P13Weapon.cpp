// Copyright © 2024, IKinder

#include "Weapon/P13Weapon.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AISense_Hearing.h"
#include "Weapon/P13ProjectileDefault.h"

AP13Weapon::AP13Weapon()
{
	bReplicates = true;
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
}

FVector AP13Weapon::GetShootLocation() const
{
	return ShootLocation->GetComponentLocation();
}

float AP13Weapon::GetShootRate() const
{
	return 60.f / WeaponSettings->RateOfFire;
}

void AP13Weapon::WeaponInit(FP13WeaponInfo* WeaponInfo, const EP13MovementState NewState, const int32 NewWeaponIndex, const FP13WeaponDynamicInfo* DynamicInfo)
{
	WeaponSettings = WeaponInfo;
	WeaponIndex = NewWeaponIndex;
	FinishReload();
	UpdateWeaponState(NewState);
	UpdateWeaponDynamicInfo(DynamicInfo);
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
}

void AP13Weapon::UpdateWeaponDynamicInfo(const FP13WeaponDynamicInfo* DynamicInfo)
{
	if (DynamicInfo)
	{
		WeaponCurrentSettings = *DynamicInfo;
	}
	else
	{
		WeaponCurrentSettings = {WeaponSettings->MaxRound};
	}
}

void AP13Weapon::SetTargetLocation(const FVector& TargetLocation)
{
	ShotTargetLocation = TargetLocation;
}

void AP13Weapon::SetFireState(const bool bFiring)
{
	bTriggerPulled = bFiring;

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
		/* Play misfire sound only if there is no ammo. */
		if (WeaponCurrentSettings.Round == 0)
		{
			SpawnEffectsAtLocation(WeaponSettings->MisfireSound, nullptr, ShootLocation->GetComponentLocation());
		}
		SetFireState(false);
		return;
	}

	/* Stop reducing bullet disperse. */
	GetWorld()->GetTimerManager().ClearTimer(DisperseTimer);
	/* Start fire. */
	const float SecondsPerShot = GetShootRate();
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ThisClass::Server_Fire, SecondsPerShot, true, SecondsPerShot);
	Server_Fire();
	LastShotTime = GetWorld()->GetTimeSeconds();
}

bool AP13Weapon::TryReload()
{
	OnWeaponReloadInit.Broadcast(WeaponCurrentSettings.Round);

	if (!CheckWeaponCanReload())
	{
		return false;
	}

	/* Stop fire, but save that weapon trigger is still pulled. */
	if (bTriggerPulled)
	{
		SetFireState(false);
		bTriggerPulled = true;
	}

	StartReload();
	return true;
}

void AP13Weapon::AbortReloading()
{
	if (!bReloading)
	{
		return;
	}

	bReloading = false;
	GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	FinishReload(false);
}

void AP13Weapon::PlayWeaponReload()
{
	PlayAnimMontage(WeaponSettings->WeaponReloadAnim);
}

void AP13Weapon::SetMaxAvailableRound(const int32 NewMaxRound)
{
	const bool bDefault = NewMaxRound < 0;
	MaxAvailableRound = bDefault ? WeaponSettings->MaxRound : NewMaxRound;
}

bool AP13Weapon::CheckWeaponCanFire() const
{
	if (!WeaponSettings)
	{
		return false;
	}
	bool bResult = true;

	const double CurrentShotTime = GetWorld()->GetTimeSeconds();
	const double TimeFromLastShot = CurrentShotTime - LastShotTime;
	const double MinTimeBetweenShots = 60.f / WeaponSettings->RateOfFire;
	bResult = bResult && (MinTimeBetweenShots < TimeFromLastShot);
	bResult = bResult && (!GetWorld()->GetTimerManager().IsTimerActive(FireTimer));
	bResult = bResult && (!bReloading);
	bResult = bResult && (WeaponCurrentSettings.Round > 0.f);

	return bResult;
}

bool AP13Weapon::CheckWeaponCanReload()
{
	bool bResult = true;
	bResult = bResult && (WeaponCurrentSettings.Round < WeaponSettings->MaxRound);
	bResult = bResult && (MaxAvailableRound > 0);
	bResult = bResult && (MaxAvailableRound > WeaponCurrentSettings.Round);

	return bResult;
}

void AP13Weapon::Fire()
{
	MakeShot();
	Multicast_ShotEffect(WeaponSettings->FireSound, WeaponSettings->FireEffect, WeaponSettings->WeaponFireAnim);
}

void AP13Weapon::MakeShot()
{
	if (WeaponCurrentSettings.Round <= 0.f)
	{
		Server_InitReload();
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
			// Line trace shot.
		}
	}
	// Multicast_OnWeaponFireBroadcast(WeaponSettings->CharFireAnim, --WeaponCurrentSettings.Round);
	OnWeaponFire.Broadcast(WeaponSettings->CharFireAnim, --WeaponCurrentSettings.Round);
	UpdateDispersion();

	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.f, GetInstigator());
}

void AP13Weapon::SpawnProjectile()
{
	const TSubclassOf<AP13ProjectileDefault> ProjectileClass = WeaponSettings->ProjectileSettings.Class;
	if (!ProjectileClass)
	{
		return;
	}

	check(ShootLocation)
	const FTransform ShotTransform = FTransform(GetFinalDirection().Rotation(), ShootLocation->GetComponentLocation());

	AP13ProjectileDefault* Bullet = GetWorld()->SpawnActorDeferred<AP13ProjectileDefault>(ProjectileClass, ShotTransform, GetOwner(), GetInstigator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (!Bullet)
	{
		return;
	}
	Bullet->SetOwner(this);
	Bullet->SetInstigator(GetOwner<APawn>());
	Bullet->InitBullet(20.f, WeaponSettings->ProjectileSettings);
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

void AP13Weapon::StartReload()
{
	bReloading = true;

	FTimerDelegate ReloadDelegate;
	ReloadDelegate.BindUObject(this, &ThisClass::FinishReload, true);
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, ReloadDelegate, WeaponSettings->ReloadTime, false);

	Multicast_PlayAnimMontage(WeaponSettings->WeaponReloadAnim);
	Multicast_OnWeaponReloadStartBroadcast(WeaponSettings->CharReloadAnim, WeaponIndex, WeaponSettings->ReloadTime);
}

void AP13Weapon::FinishReload(const bool bSuccess)
{
	if (bSuccess)
	{
		WeaponCurrentSettings.Round = MaxAvailableRound;
	}
	bReloading = false;

	/* Recover fire if weapon trigger is still pulled. */
	SetFireState(bTriggerPulled);

	/* Stop all montages (character and weapon) and update info in UI. */
	Multicast_OnWeaponReloadFinishBroadcast(WeaponCurrentSettings.Round, WeaponIndex, bSuccess);
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

void AP13Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, WeaponCurrentSettings)
	DOREPLIFETIME(ThisClass, WeaponIndex)
	DOREPLIFETIME(ThisClass, DispersionAngle)
}

void AP13Weapon::Server_Fire_Implementation()
{
	Fire();
}

void AP13Weapon::Multicast_ShotEffect_Implementation(USoundBase* FireSound, UNiagaraSystem* FireEffect, UAnimMontage* FireAnim)
{
	SpawnEffectsAtLocation(FireSound, FireEffect, ShootLocation->GetComponentLocation());
	PlayAnimMontage(FireAnim);
}

void AP13Weapon::Server_InitReload_Implementation()
{
	TryReload();
}

void AP13Weapon::Multicast_PlayAnimMontage_Implementation(UAnimMontage* Montage)
{
	PlayAnimMontage(Montage);
}

void AP13Weapon::Server_UpdateWeaponState_Implementation(const EP13MovementState NewState)
{
	UpdateWeaponState(NewState);
}

void AP13Weapon::Multicast_OnWeaponFireBroadcast_Implementation(UAnimMontage* CharFireMontage, const int32 CurrentRound)
{
	OnWeaponFire.Broadcast(CharFireMontage, CurrentRound);
}

void AP13Weapon::Multicast_OnWeaponReloadStartBroadcast_Implementation(UAnimMontage* CharFireMontage, const int32 CurrentWeaponIndex, const float ReloadingTime)
{
	OnWeaponReloadStart.Broadcast(CharFireMontage, CurrentWeaponIndex, ReloadingTime);
}

void AP13Weapon::Multicast_OnWeaponReloadFinishBroadcast_Implementation(const int32 NewRoundNum, const int32 CurrentWeaponIndex, const bool bSuccess)
{
	OnWeaponReloadFinish.Broadcast(NewRoundNum, CurrentWeaponIndex, bSuccess);
}
