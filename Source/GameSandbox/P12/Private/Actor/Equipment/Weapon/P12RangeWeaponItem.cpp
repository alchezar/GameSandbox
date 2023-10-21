// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"

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
}

void AP12RangeWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12RangeWeaponItem::FireInput()
{
	check(GetOwner()->IsA<AP12BaseCharacter>())
	AP12BaseCharacter* OwnerCharacter = StaticCast<AP12BaseCharacter*>(GetOwner());
	AController* Controller = OwnerCharacter->GetController();
	if (!Controller)
	{
		return;
	}
	FVector PlayerLocation;
	FRotator PlayerRotation;
	Controller->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	const FVector PlayerDirection = PlayerRotation.RotateVector(FVector::ForwardVector);
	
	WeaponBarrel->Shot(PlayerLocation, PlayerDirection, Controller);

	OwnerCharacter->PlayAnimMontage(CharacterFireMontage);
	PlayAnimMontage(WeaponFireMontage);
	
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
