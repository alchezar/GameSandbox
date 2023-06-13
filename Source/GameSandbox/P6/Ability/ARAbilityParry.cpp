// Copyright (C) 2023, IKinder

#include "ARAbilityParry.h"

UARAbilityParry::UARAbilityParry()
{}

void UARAbilityParry::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	FTimerHandle ParryTimer;
	FTimerDelegate ParryDelegate;
	ParryDelegate.BindUFunction(this, "StopAbility");
	GetWorld()->GetTimerManager().SetTimer(ParryTimer, ParryDelegate, 1.f, false, 1.f);
}

void UARAbilityParry::StopAbility_Implementation(AActor* Instigator)
{
	Super::StopAbility_Implementation(Instigator);
}