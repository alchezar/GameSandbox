// Copyright © 2025, Ivan Kinder

#include "Public/Character/P17CharacterBase.h"

#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "AbilitySystem/P17AttributeSet.h"

AP17CharacterBase::AP17CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	GetMesh()->bReceivesDecals = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	AbilitySystemComponent = CreateDefaultSubobject<UP17AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UP17AttributeSet>("AttributeSet");
}

void AP17CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	WARN_RETURN_IF(StartupData.IsNull(),);
	WARN_RETURN_IF(!AbilitySystemComponent,);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* AP17CharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UP17CombatPawnComponent* AP17CharacterBase::GetCombatComponent() const
{
	return nullptr;
}

void AP17CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
