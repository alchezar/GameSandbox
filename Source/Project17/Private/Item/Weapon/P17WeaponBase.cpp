// Copyright © 2025, Ivan Kinder

#include "Item/Weapon/P17WeaponBase.h"

#include "Components/BoxComponent.h"

AP17WeaponBase::AP17WeaponBase()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponStaticMeshComponent");
	WeaponMesh->SetupAttachment(GetRootComponent());

	WeaponCollisionBox = CreateDefaultSubobject<UBoxComponent>("WeaponCollisionBoxComponent");
	WeaponCollisionBox->SetupAttachment(WeaponMesh);
	WeaponCollisionBox->SetBoxExtent(FVector {20.f});
	WeaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
