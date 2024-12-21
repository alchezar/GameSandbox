// Copyright (C) 2023, IKinder

#include "Component/STU_WeaponComponent.h"

#include "Animation/STU_AnimNotify_EquipFinished.h"
#include "Animation/STU_AnimNotify_ReloadFinished.h"
#include "Animation/STU_AnimNotify_WeaponChanged.h"
#include "GameFramework/Character.h"
#include "Weapon/STU_BaseWeapon.h"

USTU_WeaponComponent::USTU_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
	InitAnimations();
}

void USTU_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (const auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

// Weapon
void USTU_WeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	for (auto [WeaponClass, ReloadAnimation] : WeaponData)
	{
		ASTU_BaseWeapon* Weapon = GetWorld()->SpawnActor<ASTU_BaseWeapon>(WeaponClass);
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &ThisClass::OnClipEmpty);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), BackSocketName);
	}
}

void USTU_WeaponComponent::StartFire()
{
	if (!CanFire()) return;

	CurrentWeapon->StartFire();
}

void USTU_WeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;

	CurrentWeapon->StopFire();
}

void USTU_WeaponComponent::Aiming()
{
	if (bWeaponChanging || bWeaponReloading) return;

	CurrentWeapon->Aiming();
}

void USTU_WeaponComponent::ToggleAim(const bool bAim)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->ToggleAim(bAim);
	}
}

void USTU_WeaponComponent::AttachWeaponToSocket(ASTU_BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTU_WeaponComponent::EquipWeapon(const int32 Index)
{
	if (CurrentWeaponIndex < 0 || CurrentWeaponIndex > Weapons.Num()) return;

	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		CurrentWeapon->ToggleAim(false);
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), BackSocketName);
	}

	CurrentWeapon          = Weapons[Index];
	CurrentReloadAnimation = WeaponData[Index].ReloadAnimation;

	// const FWeaponData* CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData Data)
	// {
	// 	return Data.WeaponClass == CurrentWeapon->GetClass();
	// });
	// CurrentReloadAnimation = CurrentWeaponData ? CurrentWeaponData->ReloadAnimation : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), HandSocketName);
}

void USTU_WeaponComponent::NextWeapon()
{
	if (GetIsWeaponChanging()) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	PlayAnimMontage(EquipAnimation);
	SetIsWeaponChanging(true);
	SetIsWeaponReloading(false);
	/* On the middle of animation will be called OnChangedFinished from notify delegate
	 * On the end of animation will be called OnEquipFinished from notify delegate*/
}

bool USTU_WeaponComponent::GetIsWeaponChanging() const
{
	return bWeaponChanging;
}

bool USTU_WeaponComponent::GetIsWeaponReloading() const
{
	return bWeaponReloading;
}

void USTU_WeaponComponent::SetIsWeaponChanging(const bool bChanging)
{
	bWeaponChanging = bChanging;
}

void USTU_WeaponComponent::SetIsWeaponReloading(const bool bReloading)
{
	bWeaponReloading = bReloading;
}

bool USTU_WeaponComponent::CanFire() const
{
	return CurrentWeapon && !bWeaponChanging && !bWeaponReloading;
}

bool USTU_WeaponComponent::CanEquip() const
{
	return !bWeaponChanging && !bWeaponReloading;
}

bool USTU_WeaponComponent::CanReload() const
{
	return CurrentWeapon && !bWeaponChanging && !bWeaponReloading && CurrentWeapon->CanReload();
}

void USTU_WeaponComponent::Reload()
{
	ChangeClip();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void USTU_WeaponComponent::OnClipEmpty(ASTU_BaseWeapon* EmptyWeapon)
{
	if (!EmptyWeapon) return;

	if (CurrentWeapon == EmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == EmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void USTU_WeaponComponent::ChangeClip()
{
	if (!CanReload()) return;

	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	PlayAnimMontage(CurrentReloadAnimation);
	SetIsWeaponReloading(true);
}

bool USTU_WeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTU_WeaponComponent::GetWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTU_WeaponComponent::TryToAddAmmo(TSubclassOf<ASTU_BaseWeapon> WeaponType, int32 Clips)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(Clips);
		}
	}
	return false;
}

bool USTU_WeaponComponent::NeedAmmo(TSubclassOf<ASTU_BaseWeapon> WeaponType)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->IsAmmoEmpty();
		}
	}
	return false;
}

// Animation
void USTU_WeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

template <typename T>
T* USTU_WeaponComponent::FindNotifyByClass(UAnimSequenceBase* Animation)
{
	if (!Animation) return nullptr;

	const TArray<FAnimNotifyEvent> NotifyEvents = Animation->Notifies;
	for (auto NotifyEvent : NotifyEvents)
	{
		if (auto AnimNotify = Cast<T>(NotifyEvent.Notify))
		{
			return AnimNotify;
		}
	}
	return nullptr;
}

void USTU_WeaponComponent::InitAnimations()
{
	// Looking for multiple notifies in one animation, same for all weapons
	if (const auto EquipFinishedNotify = FindNotifyByClass<USTU_AnimNotify_EquipFinished>(EquipAnimation))
	{
		EquipFinishedNotify->OnFinishedNotified.AddUObject(this, &ThisClass::OnEquipFinished);
	}
	if (const auto EquipChangedNotify = FindNotifyByClass<USTU_AnimNotify_WeaponChanged>(EquipAnimation))
	{
		EquipChangedNotify->OnChangedNotified.AddUObject(this, &ThisClass::OnChangedFinished);
	}

	// Looking the notify in animations, unique to each weapon
	for (auto [WeaponClass, ReloadAnimation] : WeaponData)
	{
		if (const auto EquipReloadedNotify = FindNotifyByClass<USTU_AnimNotify_ReloadFinished>(ReloadAnimation))
		{
			EquipReloadedNotify->OnReloadedNotified.AddUObject(this, &ThisClass::OnReloadFinished);
		}
	}
}

// ReSharper disable CppParameterMayBeConstPtrOrRef
void USTU_WeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	SetIsWeaponChanging(false);
}

void USTU_WeaponComponent::OnChangedFinished(USkeletalMeshComponent* MeshComp)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	EquipWeapon(CurrentWeaponIndex);
}

void USTU_WeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	SetIsWeaponReloading(false);
}
