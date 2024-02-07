// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"

#include "Net/UnrealNetwork.h"

UP13CharacterAttributesComponent::UP13CharacterAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
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

	if (bRecovering)
	{
		ShieldRecoveryTick(DeltaTime);
	}
}

void UP13CharacterAttributesComponent::ReceiveDamage(const float Damage, AController* Causer)
{
	if (FMath::IsNearlyZero(Damage))
	{
		return;
	}

	/* Stop shield restoring when we had damage. */
	bRecovering = false;

	const float ShieldBefore = Shield;
	Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
	OnShieldChanged.Broadcast(Shield, FMath::Min(Damage, ShieldBefore), Shield / MaxShield);

	const float RealDamage = (ShieldBefore < Damage) ? (Damage - ShieldBefore) : 0.f;

	/* Wait till we can start restore our shield. */
	FTimerDelegate StartRecoveryDelegate;
	StartRecoveryDelegate.BindLambda([this]() { bRecovering = true; });
	GetWorld()->GetTimerManager().SetTimer(StartRecoveryTimer, StartRecoveryDelegate, RecoveryStartDelay, false);

	Super::ReceiveDamage(RealDamage, Causer);
}

void UP13CharacterAttributesComponent::ShieldRecoveryTick(const float DeltaTime)
{
	const float TickRestore = DeltaTime / RecoveryTime * MaxShield;
	Shield = FMath::Clamp(Shield + TickRestore, 0.f, MaxShield);
	OnShieldChanged.Broadcast(Shield, 0.f, Shield / MaxShield);

	if (FMath::IsNearlyEqual(Shield, MaxShield))
	{
		bRecovering = false;
	}
}

void UP13CharacterAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Shield)
}
