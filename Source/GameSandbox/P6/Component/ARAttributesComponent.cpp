// Copyright (C) 2023, IKinder

#include "ARAttributesComponent.h"
#include "Net/UnrealNetwork.h"
#include "P6/Game/ARGameModeBase.h"

UARAttributesComponent::UARAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
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
	// AROnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	MulticastHealthChanged(InstigatorActor, Health, ActualDelta);

	if (ActualDelta < 0.f && Health == 0.f)
	{
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

void UARAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Health);
	DOREPLIFETIME_CONDITION(ThisClass, HealthMax, COND_InitialOnly);
}

void UARAttributesComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	AROnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}