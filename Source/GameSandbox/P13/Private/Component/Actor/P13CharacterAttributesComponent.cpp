// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"

UP13CharacterAttributesComponent::UP13CharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.TickInterval = 0.f;
}

void UP13CharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	Shield = MaxShield;
	OnShieldChanged.Broadcast(Shield, 0.f, 1.f);
}

void UP13CharacterAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRestoring)
	{
		ShieldRestoreTick(DeltaTime);
	}
}

void UP13CharacterAttributesComponent::ReceiveDamage(const float Damage)
{
	if (!Damage)
	{
		return;
	}
	/* Stop shield restoring when we had damage. */
	if (bRestoring)
	{
		bRestoring = false;
	}
	
	const float ShieldBefore = Shield;
	Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
	OnShieldChanged.Broadcast(Shield, FMath::Min(Damage, ShieldBefore), Shield / MaxShield);

	const float RealDamage = ShieldBefore < Damage ? (Damage - ShieldBefore) : 0.f;
	Super::ReceiveDamage(RealDamage);

	/* Wait till we can start restore our shield. */
	FTimerDelegate RestoreDelegate;
	RestoreDelegate.BindLambda([this]() { bRestoring = true; });	
	GetWorld()->GetTimerManager().SetTimer(RestoreTimer, RestoreDelegate, RestoreDelay, false);
}

void UP13CharacterAttributesComponent::ShieldRestoreTick(const float DeltaTime)
{
	const float TickRestore = DeltaTime / RestoreTime * MaxShield;
	Shield = FMath::Clamp(Shield + TickRestore, 0, MaxShield);
	OnShieldChanged.Broadcast(Shield, 0.f, Shield / MaxShield);

	if (FMath::IsNearlyEqual(Shield, MaxShield))
	{
		bRestoring = false;
	}
}
