// Copyright (C) 2023, IKinder

#include "P10/Public/Interact/Powerup/P10RegenerateHealth.h"

#include "Kismet/GameplayStatics.h"
#include "P10/Public/Component/P10HealthComponent.h"

AP10RegenerateHealth::AP10RegenerateHealth()
{
	Interval = 0.1f;
	TotalTicks = 10;
}

void AP10RegenerateHealth::OnActivated(AActor* Target)
{
	Super::OnActivated(Target);
	if (!Target) return;

	TargetActor = Target;
	HealthComponent = Target->GetComponentByClass<UP10HealthComponent>();
	
	HealthDelta = HealthComponent->GetMaxHealth() - HealthComponent->GetHealth();
	Heal = HealthDelta / TotalTicks;
}

void AP10RegenerateHealth::OnTickPowerup()
{
	Super::OnTickPowerup();
	if (! TargetActor || !HealthComponent) return;

	const float HealAmount = -1 * Heal;
	UGameplayStatics::ApplyDamage(TargetActor, HealAmount, nullptr, this, nullptr);
}