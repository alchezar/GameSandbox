// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13HealthComponent.h"

UP13HealthComponent::UP13HealthComponent()
{
}

void UP13HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health, 0, 1.f);
}

void UP13HealthComponent::ReceiveDamage(const float Damage)
{
	if (Health <= 0.f)
	{
		return;
	}
	
	const float HealthBefore = Health;
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
	OnHealthChanged.Broadcast(Health, FMath::Min(Damage, HealthBefore), Health / MaxHealth);

	if (FMath::IsNearlyZero(Health))
	{
		OnHealthOver.Broadcast();
		OnDead();
	}
}

void UP13HealthComponent::OnDead()
{
	
}
