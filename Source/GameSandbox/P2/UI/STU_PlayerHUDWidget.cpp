// Copyright (C) 2023, IKinder

#include "STU_PlayerHUDWidget.h"
#include "GameSandbox/P2/STU_CoreTypes.h"
#include "GameSandbox/P2/Component/STU_HealthComponent.h"
#include "GameSandbox/P2/Component/STU_WeaponComponent.h"

bool USTU_PlayerHUDWidget::Initialize()
{
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawnHandle);
	}
	OnNewPawnHandle(GetOwningPlayerPawn());
	return Super::Initialize();
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void USTU_PlayerHUDWidget::OnNewPawnHandle(APawn* NewPawn)
{
	if (!NewPawn) return;
	
	USTU_HealthComponent* HealthComponent = NewPawn->FindComponentByClass<USTU_HealthComponent>(); // GetOwnerComponent<USTU_HealthComponent>();
	if (HealthComponent && !HealthComponent->OnHealthChanged.IsBoundToObject(this))
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangeHandle);
	}
}

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

void USTU_PlayerHUDWidget::OnHealthChangeHandle(float Health, const float HealthDelta)
{
	if (HealthDelta < 0.f)
	{
		OnTakeDamage();
	}
}
