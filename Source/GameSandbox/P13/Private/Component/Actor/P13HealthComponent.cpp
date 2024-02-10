// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13HealthComponent.h"

#include "Net/UnrealNetwork.h"

UP13HealthComponent::UP13HealthComponent()
{
	SetIsReplicatedByDefault(true);
}

void UP13HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		Health = MaxHealth;
		Multicast_ChangeHealth(Health, 0.f, 1.f);
		// OnHealthChanged.Broadcast(Health, 0.f, 1.f);

		if (AActor* Owner = GetOwner())
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnOwnerTakeAnyDamageHandle);
		}
	}
}

void UP13HealthComponent::ReceiveDamage(const float Damage, AController* Causer)
{
	if (!bAlive || FMath::IsNearlyZero(Damage))
	{
		return;
	}

	const float HealthBefore = Health;
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	Multicast_ChangeHealth(Health, FMath::Min(Damage, HealthBefore), Health / MaxHealth);
	// OnHealthChanged.Broadcast(Health, FMath::Min(Damage, HealthBefore), Health / MaxHealth);

	if (FMath::IsNearlyZero(Health))
	{
		// OnHealthOver.Broadcast(Causer);
		Multicast_HealthOver(Causer);
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
	Multicast_ChangeHealth(Health, 0.f, Health / MaxHealth);
	// OnHealthChanged.Broadcast(Health, 0.f, Health / MaxHealth);
}

void UP13HealthComponent::ChangeHealth(const float Power, AController* Causer)
{
	if (FMath::IsNearlyZero(Power))
	{
		return;
	}

	Power > 0 ? Server_AddHealth(Power) : Server_ReceiveDamage(FMath::Abs(Power), Causer);
}

void UP13HealthComponent::OnOwnerTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Server_ReceiveDamage(Damage, InstigatedBy);
}

void UP13HealthComponent::OnDead()
{
	bAlive = false;
}

void UP13HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Health)
	DOREPLIFETIME(ThisClass, bAlive)
}

void UP13HealthComponent::Server_ChangeHealth_Implementation(const float Power, AController* Causer)
{
	ChangeHealth(Power, Causer);
}

void UP13HealthComponent::Server_AddHealth_Implementation(const float HealthAid)
{
	AddHealth(HealthAid);
}

void UP13HealthComponent::Server_ReceiveDamage_Implementation(const float Damage, AController* Causer)
{
	ReceiveDamage(Damage, Causer);
}

void UP13HealthComponent::Multicast_ChangeHealth_Implementation(const float NewHealth, const float LastDamage, const float HealthAlpha)
{
	OnHealthChanged.Broadcast(NewHealth, LastDamage, HealthAlpha);
}

void UP13HealthComponent::Multicast_HealthOver_Implementation(AController* Causer)
{
	OnHealthOver.Broadcast(Causer);
}
