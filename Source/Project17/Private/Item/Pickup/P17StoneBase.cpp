// Copyright © 2025, Ivan Kinder

#include "Item/Pickup/P17StoneBase.h"

#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "Character/P17CharacterHero.h"
#include "Util/P17GameplayTags.h"

AP17StoneBase::AP17StoneBase()
{}

void AP17StoneBase::BeginPlay()
{
	Super::BeginPlay();
}

void AP17StoneBase::OnBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const AP17CharacterHero* HeroCharacter = Cast<AP17CharacterHero>(OtherActor);
	RETURN_IF(!HeroCharacter,)

	HeroCharacter->GetProjectAbilitySystemComponent()->TryActivateAbilityByTag(P17::Tags::Player_Ability_Pickup_Stones);
}

void AP17StoneBase::Consume(UP17AbilitySystemComponent* InASC, const int32 InLevel)
{
	WARN_RETURN_IF(!GameplayEffectClass,)
	const UGameplayEffect* GameplayEffect = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	WARN_RETURN_IF(!GameplayEffect,)

	InASC->ApplyGameplayEffectToSelf(GameplayEffect, InLevel, InASC->MakeEffectContext());
	BP_OnConsumed();
}
