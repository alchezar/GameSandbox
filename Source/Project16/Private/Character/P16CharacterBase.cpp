// Copyright © 2025, Ivan Kinder

#include "Character/P16CharacterBase.h"

#include "Project16.h"
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
	InitDefaultAttributes();
}

void AP16CharacterBase::InitDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes);
	ApplyEffectToSelf(DefaultSecondaryAttributes);
}

void AP16CharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& InGameplayEffect, const float InLevel) const
{
	check(AbilitySystemComponent)
	EARLY_RETURN_IF(!InGameplayEffect)

	const FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle    Spec    = AbilitySystemComponent->MakeOutgoingSpec(InGameplayEffect, InLevel, Context);
	AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), AbilitySystemComponent.Get());
}
