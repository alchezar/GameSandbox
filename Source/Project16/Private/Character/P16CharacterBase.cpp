// Copyright © 2025, Ivan Kinder

#include "Character/P16CharacterBase.h"

#include "AbilitySystem/P16AbilitySystemComponent.h"

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

void AP16CharacterBase::InitAbilityActorInfo()
{
	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(AbilitySystemComponent))
	{
		AbilitySystem->OnAbilityActorInfoSet();
	}
}
