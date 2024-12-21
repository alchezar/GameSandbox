// Copyright (C) 2023, IKinder

#include "Ability/ARAbilityEffect.h"

#include "Component/ARAbilityComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Util/ARFuncLibrary.h"

UARAbilityEffect::UARAbilityEffect()
{
	bAutostart = true;
}

void UARAbilityEffect::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	if (Duration > 0.f)
	{
		FTimerDelegate DurationDelegate;
		DurationDelegate.BindUObject(this, &ThisClass::StopAbility, Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationTimer, DurationDelegate, Duration, false);
	}
	if (Period > 0.f)
	{
		FTimerDelegate PeriodDelegate;
		PeriodDelegate.BindUObject(this, &ThisClass::ExecutePeriodicEffect, Instigator);
		GetWorld()->GetTimerManager().SetTimer(PeriodTimer, PeriodDelegate, Period, false);
	}
	if (AbilityComponent->GetOwnerRole() == ROLE_Authority)
	{
		TimeStarted = GetWorld()->TimeSeconds;
	}
}

void UARAbilityEffect::StopAbility_Implementation(AActor* Instigator)
{
	if (GetWorld()->GetTimerManager().GetTimerRemaining(DurationTimer) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAbility_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(DurationTimer);
	GetWorld()->GetTimerManager().ClearTimer(PeriodTimer);

	UARAbilityComponent* AbilityComp = GetAbilityComponent();
	if (!AbilityComp) return;

	AbilityComp->RemoveAbility(this);
}

void UARAbilityEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	const UARAbilityComponent* AbilityComp = GetAbilityComponent();
	if (!AbilityComp) return;
	AActor* Owner = AbilityComp->GetOwner();
	if (!Owner) return;

	UARFuncLibrary::ApplyDamage(Instigator, Owner, DamageAmount);
}


float UARAbilityEffect::GetTimeRemaining() const
{
	const AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	if (!GameState) return Duration;
	
	const float EndTime = TimeStarted + Duration;
	return EndTime - GameState->GetServerWorldTimeSeconds();
}

void UARAbilityEffect::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TimeStarted);
}
