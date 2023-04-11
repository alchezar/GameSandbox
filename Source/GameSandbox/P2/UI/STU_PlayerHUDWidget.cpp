// Copyright (C) 2023, IKinder

#include "STU_PlayerHUDWidget.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "GameSandbox/P2/Component/STU_WeaponComponent.h"


float USTU_PlayerHUDWidget::GetHealthPercent() const
{
	return GetOwnerComponent<USTU_HealthComponent>()->GetHealthPercent();
}

bool USTU_PlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	return GetOwnerComponent<USTU_WeaponComponent>()->GetWeaponUIData(UIData);
}

bool USTU_PlayerHUDWidget::GetCurrentAmmoData(FAmmoData& AmmoData) const
{
	return GetOwnerComponent<USTU_WeaponComponent>()->GetWeaponAmmoData(AmmoData);
}

bool USTU_PlayerHUDWidget::IsPlayerAlive() const
{
	const auto HealthComponent = GetOwnerComponent<USTU_HealthComponent>();
	return HealthComponent && !HealthComponent->IsDead();
}

bool USTU_PlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating; 
}

template<class T>
T* USTU_PlayerHUDWidget::GetOwnerComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return nullptr;

	T* Component = Cast<T>(Player->GetComponentByClass(T::StaticClass()));
	if (!Component) return nullptr;

	return Component;
}
