// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13HealthComponent.h"

UP13HealthComponent::UP13HealthComponent()
{
	
}

void UP13HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health, 0.f, 1.f);
}

void UP13HealthComponent::ReceiveDamage(const float Damage)
{
	if (FMath::IsNearlyZero(Health) || FMath::IsNearlyZero(Damage))
	{
		return;
	}

	const float HealthBefore = Health;
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, FMath::Min(Damage, HealthBefore), Health / MaxHealth);

	if (FMath::IsNearlyZero(Health))
	{
		OnHealthOver.Broadcast();
		OnDead();
	}
}

void UP13HealthComponent::AddHealth(const float HealthAid)
{
	if (FMath::IsNearlyZero(HealthAid))
	{
		return;
	}
	Health = FMath::Clamp(Health + HealthAid, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, 0.f, Health / MaxHealth);
}

void UP13HealthComponent::ChangeHealth(const float Power)
{
	if (FMath::IsNearlyZero(Power))
	{
		return;	
	}

	Power > 0 ? AddHealth(Power) : ReceiveDamage(FMath::Abs(Power));
}

void UP13HealthComponent::OnDead()
{}
