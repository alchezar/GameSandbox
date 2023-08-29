// Copyright (C) 2023, IKinder

#include "P10/Public/Component/P10HealthComponent.h"
#include "GameFramework/Actor.h"

UP10HealthComponent::UP10HealthComponent()
{
	
}

void UP10HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* OwnerActor = GetOwner())
	{
		OwnerActor->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeDamageHandle);
	}
	Health = MaxHealth;
}

void UP10HealthComponent::OnTakeDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) return;

	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}
