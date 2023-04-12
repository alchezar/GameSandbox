// Copyright (C) 2023, IKinder

#include "STU_HealthComponent.h"
#include "GameFramework/Actor.h"

USTU_HealthComponent::USTU_HealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->OnTakeAnyDamage.AddDynamic(this, &USTU_HealthComponent::OnTakeAnyDamageHandle);
}

float USTU_HealthComponent::GetHealth() const
{
	return Health;
}

bool USTU_HealthComponent::TryToAddHealth(const float PickedHealth)
{
	if (IsDead() || IsHealthFull()) return false;
	
	SetHealth(Health + PickedHealth);
	return true;
}

bool USTU_HealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
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

float USTU_HealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void USTU_HealthComponent::SetHealth(const float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

void USTU_HealthComponent::Healing()
{
	SetHealth(Health + HealAmount);
	if (IsHealthFull())
	{
		GetWorld()->GetTimerManager().ClearTimer(OUT HealTimer);
	}
}
