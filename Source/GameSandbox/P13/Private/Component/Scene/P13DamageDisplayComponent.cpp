// Copyright © 2024, IKinder

#include "P13/Public/Component/Scene/P13DamageDisplayComponent.h"

#include "Blueprint/UserWidget.h"
#include "P13/Public/Component/Actor/P13HealthComponent.h"
#include "P13/Public/UI/Damage/P13DamageDisplayWidget.h"

UP13DamageDisplayComponent::UP13DamageDisplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP13DamageDisplayComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UP13HealthComponent* HealthComponent = GetOwner() ? GetOwner()->FindComponentByClass<UP13HealthComponent>() : nullptr)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnOwnerHealthChangedHandle);
	}
}

void UP13DamageDisplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP13DamageDisplayComponent::DisplayDamage(const float CurrentDamage, const float HealthAlpha)
{
	if (!DamageDisplayWidgetClass || FMath::IsNearlyZero(CurrentDamage))
	{
		return;
	}

	FVector2D Offset;
	if (UP13DamageDisplayWidget* DamageDisplayWidget = CreateDamageWidget(Offset))
	{
		DamageDisplayWidget->SetupDamageWidget(this, CurrentDamage, HealthAlpha, Offset);
	}
}

void UP13DamageDisplayComponent::DisplayShield(const float CurrentDamage, const float ShieldAlpha)
{
	if (FMath::IsNearlyZero(CurrentDamage) || FMath::IsNearlyZero(ShieldAlpha))
	{
		return;
	}

	FVector2D Offset;
	if (UP13DamageDisplayWidget* DamageDisplayWidget = CreateDamageWidget(Offset))
	{
		DamageDisplayWidget->SetupShieldWidget(this, "Shield", Offset);
	}
}

UP13DamageDisplayWidget* UP13DamageDisplayComponent::CreateDamageWidget(FVector2D& Offset) const
{
	if (!DamageDisplayWidgetClass)
	{
		return nullptr;
	}

	UP13DamageDisplayWidget* DamageDisplayWidget = CreateWidget<UP13DamageDisplayWidget>(GetWorld(), DamageDisplayWidgetClass);
	if (!DamageDisplayWidget)
	{
		return nullptr;
	}
	DamageDisplayWidget->AddToViewport();
	Offset = FVector2D(FMath::RandRange(-20.f, 20.f), FMath::RandRange(-20.f, 20.f));

	return DamageDisplayWidget;
}

void UP13DamageDisplayComponent::OnOwnerHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha)
{
	DisplayDamage(LastDamage, HealthAlpha);
}
