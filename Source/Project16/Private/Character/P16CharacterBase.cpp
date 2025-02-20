// Copyright © 2025, Ivan Kinder

#include "Character/P16CharacterBase.h"

AP16CharacterBase::AP16CharacterBase()
{
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	Weapon->SetupAttachment(GetMesh(), HandSocketName);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AP16CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
