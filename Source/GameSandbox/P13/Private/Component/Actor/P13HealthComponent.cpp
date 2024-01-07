// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13HealthComponent.h"

UP13HealthComponent::UP13HealthComponent()
{
}

void UP13HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void UP13HealthComponent::ReceiveDamage(const float Damage)
{
	if (Health <= 0.f)
	{
		return;
	}
	
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
	OnHealthChanged.Broadcast(Health, Damage, Health / MaxHealth);

	if (Damage >= Health)
	{
		OnHealthOver.Broadcast();
		OnDead();
	}
}

void UP13HealthComponent::OnDead()
{
	
}
