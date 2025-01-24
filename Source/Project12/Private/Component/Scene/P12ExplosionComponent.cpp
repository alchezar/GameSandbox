// Copyright (C) 2023, IKinder

#include "Component/Scene/P12ExplosionComponent.h"

#include "Kismet/GameplayStatics.h"

void UP12ExplosionComponent::Explode(AController* Instigator)
{
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage.Max, Damage.Min, GetComponentLocation(), Radius.Min, Radius.Max, DamageFallow, DamageTypeClass, {}, GetOwner(), Instigator);

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetComponentLocation());
	}

	OnExplosion.Broadcast();
}
