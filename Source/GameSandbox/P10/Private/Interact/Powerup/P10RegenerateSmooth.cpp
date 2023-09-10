// Copyright (C) 2023, IKinder

#include "P10/Public/Interact/Powerup/P10RegenerateSmooth.h"

#include "Kismet/GameplayStatics.h"

AP10RegenerateSmooth::AP10RegenerateSmooth()
{
}

void AP10RegenerateSmooth::ActivatePowerup(AActor* Target)
{
	Super::ActivatePowerup(Target);

	OnActivated(Target);

	const float Rate =  GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(PowerupTimer, this, &ThisClass::OnTickPowerup, Rate, true);
}

void AP10RegenerateSmooth::OnActivated(AActor* Target)
{
	Super::OnActivated(Target);
	
	Heal = HealthDelta / Duration;;
}

void AP10RegenerateSmooth::OnTickPowerup()
{
	Duration -= GetWorld()->GetDeltaSeconds();
	
	if (Duration <= 0.f)
	{
		OnExpired();
	}
	if (TargetActor && HealthComponent)
	{
		const float HealAmount = -1 * Heal * GetWorld()->GetDeltaSeconds();
		UGameplayStatics::ApplyDamage(TargetActor, HealAmount, nullptr, this, nullptr);
	}
}
