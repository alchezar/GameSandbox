// Copyright © 2025, Ivan Kinder

#include "Character/P16Enemy.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"

AP16Enemy::AP16Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// For enemies highlight on hover.
	GetMesh()->SetCustomDepthStencilValue(P16::CustomDepthRed);
	Weapon->SetCustomDepthStencilValue(P16::CustomDepthRed);

	AbilitySystemComponent = CreateDefaultSubobject<UP16AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UP16AttributeSet>("AttributeSetComponent");
}

void AP16Enemy::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AP16Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP16Enemy::ToggleHighlight(const bool bOn)
{
	GetMesh()->SetRenderCustomDepth(bOn);
	Weapon->SetRenderCustomDepth(bOn);
}
