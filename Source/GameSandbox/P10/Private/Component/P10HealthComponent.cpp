// Copyright (C) 2023, IKinder

#include "P10/Public/Component/P10HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

UP10HealthComponent::UP10HealthComponent()
{
	SetIsReplicatedByDefault(true);
}

void UP10HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/* Only hook when we are the server. */
	AActor* OwnerActor = GetOwner();
	if (OwnerActor && OwnerActor->HasAuthority())
	{
		OwnerActor->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeDamageHandle);
	}
	Health = MaxHealth;
}

void UP10HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Health)
	DOREPLIFETIME_CONDITION(ThisClass, MaxHealth, COND_InitialOnly)
}

void UP10HealthComponent::OnTakeDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) return;

	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
	Multicast_OnHealthChanged(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	// OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UP10HealthComponent::Multicast_OnHealthChanged_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}
