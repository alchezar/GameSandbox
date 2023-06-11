// Copyright (C) 2023, IKinder

#include "ARAttributesComponent.h"

#include "P6/Game/ARGameModeBase.h"
#include "P6/Player/ARCharacter.h"

UARAttributesComponent::UARAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UARAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = HealthMax;
}

void UARAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UARAttributesComponent::TryChangeHealth(AActor* InstigatorActor, const float Delta)
{
	if (!GetOwner()->CanBeDamaged()) return false;
	
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, 0.f, HealthMax);

	const float ActualDelta = Health - OldHealth;
	AROnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

	if (ActualDelta < 0.f && Health == 0.f)
	{
		AROnDead.Broadcast(GetOwner());
		
		AARGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AARGameModeBase>();
		if (!GameMode) return false;

		GameMode->OnPlayerKilled(GetOwner(), InstigatorActor);
	}
	
	return ActualDelta != 0.f;
}

bool UARAttributesComponent::GetIsAlive() const
{
	return Health > 0.f;
}

bool UARAttributesComponent::GetIsHealthMax() const
{
	return Health == HealthMax;
}

float UARAttributesComponent::GetHealthMax() const
{
	return HealthMax;
}

float UARAttributesComponent::GetHealth() const
{
	return Health;
}

UARAttributesComponent* UARAttributesComponent::GetAttributes(AActor* FromActor)
{
	if (!FromActor) return nullptr;

	return FromActor->FindComponentByClass<UARAttributesComponent>();
}

bool UARAttributesComponent::GetIsActorAlive(AActor* Actor)
{
	const UARAttributesComponent* AttributesComp = GetAttributes(Actor);
	if (!AttributesComp) return false;

	return AttributesComp->GetIsAlive();
}
