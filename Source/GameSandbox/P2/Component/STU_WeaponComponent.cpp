// Copyright (C) 2023, IKinder

#include "STU_WeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameSandbox/P2/Animation/STU_AnimNotify_EquipFinished.h"
#include "GameSandbox/P2/Animation/STU_AnimNotify_ReloadFinished.h"
#include "GameSandbox/P2/Animation/STU_AnimNotify_WeaponChanged.h"
#include "GameSandbox/P2/Weapon/STU_BaseWeapon.h"

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

#pragma region Weapon

void USTU_WeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

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
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), BackSocketName);
	}

	CurrentWeapon = Weapons[Index];
	// CurrentReloadAnimation = WeaponData[Index].ReloadAnimation;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData Data)
	{
		return Data.WeaponClass == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimation = CurrentWeaponData ? CurrentWeaponData->ReloadAnimation : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), HandSocketName);
}

void USTU_WeaponComponent::NextWeapon()
{
	if (IsWeaponChanging()) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	PlayAnimMontage(EquipAnimation);
	bWeaponChanging  = true;
	bWeaponReloading = false;
	// On the middle of animation will be called OnChangedFinished from notify delegate
	// On the end of animation will be called OnEquipFinished from notify delegate
}

bool USTU_WeaponComponent::IsWeaponChanging() const
{
	return bWeaponChanging;
}

bool USTU_WeaponComponent::IsWeaponReloading() const
{
	return bWeaponReloading;
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
	bWeaponReloading = true;
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

#pragma endregion // Weapon

#pragma  region Animation

void USTU_WeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void USTU_WeaponComponent::InitAnimations()
{
	if (EquipAnimation)
	{
		TArray<FAnimNotifyEvent> NotifyEvents = EquipAnimation->Notifies;
		for (const FAnimNotifyEvent NotifyEvent : NotifyEvents)
		{
			const auto EquipFinishedNotify = Cast<USTU_AnimNotify_EquipFinished>(NotifyEvent.Notify);
			if (EquipFinishedNotify)
			{
				EquipFinishedNotify->OnFinishedNotified.AddUObject(this, &ThisClass::OnEquipFinished);
				break;
			}

			const auto EquipChangedNotify = Cast<USTU_AnimNotify_WeaponChanged>(NotifyEvent.Notify);
			if (EquipChangedNotify)
			{
				EquipChangedNotify->OnChangedNotified.AddUObject(this, &ThisClass::OnChangedFinished);
			}
		}
	}
	if (CurrentReloadAnimation)
	{
		TArray<FAnimNotifyEvent> NotifyEvents = CurrentReloadAnimation->Notifies;
		for (const FAnimNotifyEvent NotifyEvent : NotifyEvents)
		{
			const auto EquipReloadedNotify = Cast<USTU_AnimNotify_ReloadFinished>(NotifyEvent.Notify);
			if (EquipReloadedNotify)
			{
				EquipReloadedNotify->OnReloadedNotified.AddUObject(this, &ThisClass::OnReloadFinished);
			}
		}
	}
}

// ReSharper disable CppParameterMayBeConstPtrOrRef
void USTU_WeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	bWeaponChanging = false;
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

	bWeaponReloading = false;
}

#pragma endregion // Animation
