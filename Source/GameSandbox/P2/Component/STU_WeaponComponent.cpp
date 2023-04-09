// Copyright (C) 2023, IKinder

#include "STU_WeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameSandbox/P2/Animation/STU_AnimNotify_EquipFinished.h"
#include "GameSandbox/P2/Animation/STU_AnimNotify_WeaponChanged.h"
#include "GameSandbox/P2/Weapon/STU_BaseWeapon.h"


USTU_WeaponComponent::USTU_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
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

void USTU_WeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	for (auto WeaponClass : WeaponClasses)
	{
		ASTU_BaseWeapon* Weapon = GetWorld()->SpawnActor<ASTU_BaseWeapon>(WeaponClass);
		if (!Weapon) continue;

		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), BackSocketName);
	}
}

void USTU_WeaponComponent::StartFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StartFire();
}

void USTU_WeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void USTU_WeaponComponent::AttachWeaponToSocket(ASTU_BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTU_WeaponComponent::EquipWeapon(const int32 Index)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), BackSocketName);
	}

	CurrentWeapon = Weapons[Index];
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), HandSocketName);
	PlayAnimMontage(EquipAnimation);
}

void USTU_WeaponComponent::NextWeapon()
{
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTU_WeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void USTU_WeaponComponent::InitAnimations()
{
	if (!EquipAnimation) return;

	const auto NotifyEvents = EquipAnimation->Notifies;
	for (const auto NotifyEvent : NotifyEvents)
	{
		const auto EquipFinishedNotify = Cast<USTU_AnimNotify_EquipFinished>(NotifyEvent.Notify);
		if (EquipFinishedNotify)
		{
			EquipFinishedNotify->OnFinishedNotified.AddUObject(this, &USTU_WeaponComponent::OnEquipFinished);
			break;
		}
		const auto EquipChangedNotify = Cast<USTU_AnimNotify_WeaponChanged>(NotifyEvent.Notify);
		if (EquipChangedNotify)
		{
			EquipChangedNotify->OnChangedNotified.AddUObject(this, &USTU_WeaponComponent::OnChangedFinished);
		}
	}
}

void USTU_WeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if( Character->GetMesh() == MeshComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Equiq finished")));
	}
}

void USTU_WeaponComponent::OnChangedFinished(USkeletalMeshComponent* MeshComp)
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if( Character->GetMesh() == MeshComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Equiq changed")));
	}
}
