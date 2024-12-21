// Copyright (C) 2023, IKinder

#include "UI/STU_PlayerHUDWidget.h"

#include "Component/STU_HealthComponent.h"
#include "Component/STU_WeaponComponent.h"
#include "Components/ProgressBar.h"
#include "Game/STU_CoreTypes.h"

void  USTU_PlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::OnNewPawnHandle);
	}
	OnNewPawnHandle(GetOwningPlayerPawn());
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
	UpdateHealthBar();
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

FString USTU_PlayerHUDWidget::FormatBullet(const int32 BulletNum)
{
	constexpr int32 MaxLen = 3;
	constexpr TCHAR PrefixSymbol = '0';

	FString BulletString = FString::FromInt(BulletNum);
	const auto SymbolsToAdd = MaxLen - BulletString.Len();

	if (SymbolsToAdd > 0)
	{
		BulletString = FString::ChrN(SymbolsToAdd, PrefixSymbol).Append(BulletString);
	}
	return BulletString;
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
		// OnTakeDamage();
		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
		
	}
	UpdateHealthBar();
}

void USTU_PlayerHUDWidget::UpdateHealthBar()
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > ColorThreshold ? GoodColor : BadColor);
	}
}
