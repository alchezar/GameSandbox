// Copyright (C) 2023, IKinder

#include "Component/P10HealthComponent.h"

#include "Game/P10GameMode.h"
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
	if (Damage == 0 || Health == 0.f)
	{
		return;
	}
	
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	Multicast_OnHealthChanged(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

	const AP10GameMode* GameMode = GetWorld()->GetAuthGameMode<AP10GameMode>();
	if ( Health == 0.f && GameMode)
	{
		GameMode->OnActorKilled.Broadcast(GetOwner(), DamageCauser->GetOwner(), DamageCauser->GetInstigatorController());
	}
}

void UP10HealthComponent::Multicast_OnHealthChanged_Implementation(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UP10HealthComponent::OnRep_Health(const float OldHealth)
{
	const float Damage = Health - OldHealth;
	OnHealthChanged.Broadcast(this, Health, Damage, nullptr, nullptr, nullptr);
}

bool UP10HealthComponent::IsFriendly(const AActor* ActorA, const AActor* ActorB)
{
	/* Allows self damage. */
	if (ActorA == ActorB)
	{
		return false;
	}
	if (!ActorA || !ActorB)
	{
		return true;
	}
	const UP10HealthComponent* HealthCompA = ActorA->FindComponentByClass<UP10HealthComponent>();
	const UP10HealthComponent* HealthCompB = ActorB->FindComponentByClass<UP10HealthComponent>();
	if (!HealthCompA || !HealthCompB)
	{
		return true;
	}

	return HealthCompA->TeamNum == HealthCompB->TeamNum;
}
