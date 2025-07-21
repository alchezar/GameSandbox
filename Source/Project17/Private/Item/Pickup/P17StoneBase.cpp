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
	AP17CharacterHero* HeroCharacter = Cast<AP17CharacterHero>(OtherActor);
	RETURN_IF(!HeroCharacter,)

	HeroCharacter->GetProjectAbilitySystemComponent()->TryActivateAbilityByTag(P17::Tags::Player_Ability_Pickup_Stones);
}
