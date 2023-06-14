// Copyright (C) 2023, IKinder

#include "ARAbilityEffect.h"
// #include "Kismet/GameplayStatics.h"
// #include "Engine/DamageEvents.h"
#include "P6/Component/ARAbilityComponent.h"
// #include "P6/Component/ARAttributesComponent.h"
#include "P6/Util/ARFuncLibrary.h"

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
