// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"

#include "P12/Public/Component/Actor/P12EquipmentComponent.h"
#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Player/AnimNotify/P12AnimNotify_Reloaded.h"

AP12RangeWeaponItem::AP12RangeWeaponItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneWeaponRootComponent");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetRelativeLocation(WeaponMeshOffset);
	WeaponMesh->SetRelativeRotation(WeaponMeshOrientation);
	
	WeaponBarrel = CreateDefaultSubobject<UP12WeaponBarrelComponent>("WeaponBarrelSceneComponent");
	WeaponBarrel->SetupAttachment(WeaponMesh, MuzzleSocketName);
}

void AP12RangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner()->IsA<AP12BaseCharacter>())
	CachedCharacter = StaticCast<AP12BaseCharacter*>(GetOwner());
	InitAnimNotify();
	
	SetAmmo(MaxAmmo);
}

void AP12RangeWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12RangeWeaponItem::FireInput(const bool bStart)
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimer);
	if (bStart)
	{
		MakeShot();
		if (FireMode < EP12FireMode::FullAuto)
		{
			return;
		}
		GetWorld()->GetTimerManager().SetTimer(ShotTimer, this, &ThisClass::MakeShot, GetShotTimeInterval(), true);
	}
}

void AP12RangeWeaponItem::AimInput(const bool bStart)
{
	bAiming = bStart;
}

void AP12RangeWeaponItem::InitAnimNotify()
{
	TArray<FAnimNotifyEvent> NotifyEvents = CharacterReloadMontage->Notifies;
	for (const FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UP12AnimNotify_Reloaded* ReloadedNotify = Cast<UP12AnimNotify_Reloaded>(NotifyEvent.Notify))
		{
			ReloadedNotify->OnReloaded.AddUObject(this, &ThisClass::OnReloadedHandle);
		}
	}
}

float AP12RangeWeaponItem::PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate, const FName StartSectionName) const
{
	UAnimInstance* WeaponAnimInstance = WeaponMesh->GetAnimInstance();
	if (!WeaponAnimInstance)
	{
		return 0.f;
	}
	const float Duration = WeaponAnimInstance->Montage_Play(AnimMontage, InPlayRate);
	if (Duration <= 0)
	{
		return 0.f;
	}
	if( StartSectionName != NAME_None )
	{
		WeaponAnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
	}
	return Duration;
}

FTransform AP12RangeWeaponItem::GetGripTransform() const
{
	return WeaponMesh->GetSocketTransform("Grip_Socket");
}

float AP12RangeWeaponItem::GetShotTimeInterval()
{
	return 60.f / ShotRate;
}

void AP12RangeWeaponItem::MakeShot()
{
	if (!GetCanShoot())
	{
		FireInput(false);
		if (Ammo == 0 && bAutoReload)
		{
			Reload();
		}
		return;
	}
	check(CachedCharacter.IsValid())
	CachedCharacter = StaticCast<AP12BaseCharacter*>(GetOwner());
	AController* Controller = CachedCharacter->GetController();
	if (!Controller)
	{
		return;
	}
	FVector PlayerLocation;
	FRotator PlayerRotation;
	Controller->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	const FVector PlayerDirection = PlayerRotation.RotateVector(FVector::ForwardVector);

	CurrentBulletSpread = CachedCharacter->GetIsAiming() ? AimBulletSpread : BulletSpread;
	const float HalfAngleRad = FMath::DegreesToRadians(CurrentBulletSpread / 2.f);
	const FVector SpreadDirection = FMath::VRandCone(PlayerDirection, HalfAngleRad);

	SetAmmo(--Ammo);
	WeaponBarrel->Shot(PlayerLocation, SpreadDirection, Controller);

	CachedCharacter->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);
}

void AP12RangeWeaponItem::SetAmmo(const int32 NewAmmo) 
{
	Ammo = FMath::Clamp(NewAmmo, 0, MaxAmmo);
	const int32 AvailableAmmoCount = CachedCharacter->GetEquipmentComponent()->GetMaxAvailableAmmoAmount(AmmoType);
	CachedCharacter->OnAmmoCountChanged.Broadcast(Ammo, AvailableAmmoCount);
}

bool AP12RangeWeaponItem::GetCanShoot() const 
{
	return Ammo > 0 && !bReloading;
}

void AP12RangeWeaponItem::Reload()
{
	const bool bAmmoEmpty = CachedCharacter->GetEquipmentComponent()->GetMaxAvailableAmmoAmount(AmmoType) <= 0;
	if (bReloading || bAmmoEmpty)
	{
		return;
	}
	bReloading = true;
	if (!CharacterReloadMontage)
	{
		OnReloadedHandle(CachedCharacter->GetMesh());
		return;
	}
	CachedCharacter->PlayAnimMontage(CharacterReloadMontage);
	PlayAnimMontage(WeaponReloadMontage);
}

void AP12RangeWeaponItem::OnReloadedHandle(USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (CachedCharacter->GetMesh() != SkeletalMeshComponent || !bReloading)
	{
		return;
	}
	bReloading = false;
	UP12EquipmentComponent* EquipmentComponent = CachedCharacter->GetEquipmentComponent();
	check(EquipmentComponent)
	
	const int32 AvailableAmmoCount = EquipmentComponent->GetMaxAvailableAmmoAmount(AmmoType);
	const int32 AmmoToReload = MaxAmmo - Ammo;
	const int32 ReloadedAmmo = FMath::Min(AvailableAmmoCount, AmmoToReload);
	EquipmentComponent->DecreaseMaxAvailableAmmoAmount(AmmoType, ReloadedAmmo);
	
	SetAmmo(Ammo + ReloadedAmmo);
	CachedCharacter->OnReloadComplete.Broadcast(true);
}
