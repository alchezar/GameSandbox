// Copyright (C) 2023, IKinder

#include "STU_PlayerHUDWidget.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "GameSandbox/P2/Component/STU_WeaponComponent.h"

float USTU_PlayerHUDWidget::GetHealthPercent() const
{
	const USTU_HealthComponent* HealthComponent = GetOwnerComponent<USTU_HealthComponent>();
	return HealthComponent ? HealthComponent->GetHealthPercent() : 0.f;
}

bool USTU_PlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const USTU_WeaponComponent* WeaponComponent = GetOwnerComponent<USTU_WeaponComponent>();
	return WeaponComponent ? WeaponComponent->GetWeaponUIData(UIData) : false;
}

bool USTU_PlayerHUDWidget::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
	const USTU_WeaponComponent* WeaponComponent = GetOwnerComponent<USTU_WeaponComponent>();
	return WeaponComponent ? WeaponComponent->GetWeaponAmmoData(AmmoData) : false;
}

bool USTU_PlayerHUDWidget::IsPlayerAlive() const
{
	const USTU_HealthComponent* HealthComponent = GetOwnerComponent<USTU_HealthComponent>();
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTU_PlayerHUDWidget::IsPlayerSpectating() const
{
	const APlayerController* Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

template <class T>
T* USTU_PlayerHUDWidget::GetOwnerComponent() const
{
	const APawn* Player = GetOwningPlayerPawn();
	return Player ? Cast<T>(Player->GetComponentByClass(T::StaticClass())) : nullptr;
}
