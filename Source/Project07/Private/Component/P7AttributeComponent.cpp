// Copyright (C) 2023, IKinder

#include "Component/P7AttributeComponent.h"

#include "GameFramework/Character.h"
#include "Player/P7BaseCharacter.h"

UP7AttributeComponent::UP7AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP7AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP7AttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/* Refill stamina only after the character has stopped blocking */
	if (GetStaminaPercent() < 1.f && !Character->GetIsBlocked())
	{
		RefillStamina(StaminaRegenRate * DeltaTime);
	}
}

void UP7AttributeComponent::ReceiveDamage(const float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
	OnReceiveDamage.Broadcast(Character, GetHealthPercent());
}

void UP7AttributeComponent::UseStamina(const float StaminaCost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - StaminaCost, 0.f, MaxStamina);
	OnStaminaUsed.Broadcast(Character, GetStaminaPercent());
}

void UP7AttributeComponent::RefillStamina(const float StaminaRate)
{
	UseStamina(-1 * StaminaRate);
}

void UP7AttributeComponent::AddCoins(const int32 NewCoins)
{
	Coins += NewCoins;
	OnReceiveGold.Broadcast(Character, NewCoins);
}


void UP7AttributeComponent::AddSoul(const int32 NewSouls)
{
	Souls+= NewSouls;
	OnReceiveSoul.Broadcast(Character, NewSouls);
}

void UP7AttributeComponent::SetDefaultAttributes(ACharacter* OwnerCharacter)
{
	ResetHealth();
	ResetStamina();
	SetCharacter(OwnerCharacter);
}

void UP7AttributeComponent::ResetHealth()
{
	CurrentHealth = MaxHealth;
}

void UP7AttributeComponent::ResetStamina()
{
	CurrentStamina = MaxStamina;
}

void UP7AttributeComponent::SetCharacter(ACharacter* OwnerCharacter)
{
	Character = Cast<AP7BaseCharacter>(OwnerCharacter);
}