// Copyright © 2025, Ivan Kinder

#include "Player/P16PlayerState.h"

#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "Net/UnrealNetwork.h"

AP16PlayerState::AP16PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UP16AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UP16AttributeSet>("AttributeSetComponent");
}

void AP16PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, XP)
	DOREPLIFETIME(ThisClass, Level)
	DOREPLIFETIME(ThisClass, AttributePoints)
	DOREPLIFETIME(ThisClass, SpellPoints)
}

UAbilitySystemComponent* AP16PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UAttributeSet* AP16PlayerState::GetAttributeSet() const
{
	return AttributeSet.Get();
}

void AP16PlayerState::SetXP(const int32 NewXP)
{
	XP = NewXP;
	OnXPChanged.Broadcast(XP);
}

void AP16PlayerState::SetLevel(const int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level, false);
}

void AP16PlayerState::SetSpellPoints(const int32 NewSpellPoints)
{
	SpellPoints = NewSpellPoints;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

void AP16PlayerState::SetAttributePoints(const int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AP16PlayerState::AddXP(const int32 DeltaXP)
{
	XP += DeltaXP;
	OnXPChanged.Broadcast(XP);
}

void AP16PlayerState::AddLevel(const int32 DeltaLevel)
{
	Level += DeltaLevel;
	OnLevelChanged.Broadcast(Level, true);
}

inline void AP16PlayerState::AddAttributePoints(const int32 DeltaAttributePoints)
{
	AttributePoints += DeltaAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AP16PlayerState::AddSpellPoints(const int32 DeltaSpellPoints)
{
	SpellPoints += DeltaSpellPoints;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

void AP16PlayerState::OnRep_Level(const int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level, true);
}

void AP16PlayerState::OnRep_XP(const int32 OldXP)
{
	OnXPChanged.Broadcast(XP);
}

void AP16PlayerState::OnRep_AttributePoints(const int32 OldAttributePoints)
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AP16PlayerState::OnRep_SpellPoints(const int32 OldSpellPoints)
{
	OnSpellPointsChanged.Broadcast(SpellPoints);
}
