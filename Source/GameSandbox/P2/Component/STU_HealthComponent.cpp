// Copyright (C) 2023, IKinder

#include "STU_HealthComponent.h"
#include "GameFramework/Actor.h"

#define OUT

USTU_HealthComponent::USTU_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);

	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->OnTakeAnyDamage.AddDynamic(this, &USTU_HealthComponent::OnTakeAnyDamageHandle);
}

float USTU_HealthComponent::GetHealth() const
{
	return Health;
}

void USTU_HealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, const float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || IsDead() || !GetWorld()) return;
	SetHealth(Health - Damage);
	OnHealthChanged.Broadcast(Health);

	if (IsDead())
	{
		OnDeath.Broadcast();
		GetWorld()->GetTimerManager().ClearTimer(OUT HealTimer);
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(OUT HealTimer, this, &USTU_HealthComponent::Healing, HealUpdateDelay, true, HealStartDelay);
	}
}

bool USTU_HealthComponent::IsDead() const
{
	return FMath::IsNearlyZero(Health);
}

void USTU_HealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void USTU_HealthComponent::Healing()
{
	SetHealth(Health + HealAmount);
	if (FMath::IsNearlyEqual(Health, MaxHealth))
	{
		GetWorld()->GetTimerManager().ClearTimer(OUT HealTimer);
	}
}
