// Copyright © 2024, IKinder

#include "P13/Public/Component/Scene/P13DamageDisplayComponent.h"

#include "Blueprint/UserWidget.h"
#include "P13/Public/UI/Damage/P13DamageDisplayWidget.h"

UP13DamageDisplayComponent::UP13DamageDisplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP13DamageDisplayComponent::BeginPlay()
{
	Super::BeginPlay();
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
	
	UP13DamageDisplayWidget* DamageDisplayWidget = CreateWidget<UP13DamageDisplayWidget>(GetWorld(), DamageDisplayWidgetClass);
	if (!DamageDisplayWidget)
	{
		return;
	}
	const FVector2D RandomOffset = FVector2D(FMath::RandRange(-20.f, 20.f), FMath::RandRange(-20.f, 20.f));
	DamageDisplayWidget->InitWidget(this, CurrentDamage, HealthAlpha, RandomOffset);
	DamageDisplayWidget->AddToViewport();
}

void UP13DamageDisplayComponent::DisplayShield(const float LastDamage, const float ShieldAlpha)
{
	if (ShieldAlpha <= 0.f || LastDamage <= 0.f)
	{
		return;
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Shield");	
}

