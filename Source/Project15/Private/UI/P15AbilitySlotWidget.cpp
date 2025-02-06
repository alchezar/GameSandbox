// Copyright © 2025, Ivan Kinder

#include "UI/P15AbilitySlotWidget.h"

#include "Project15.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UP15AbilitySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	EARLY_RETURN_IF(!ImageMaterial)
	AbilityImage->SetBrushFromMaterial(ImageMaterial.Get());

	DynamicMaterial = AbilityImage->GetDynamicMaterial();
	check(DynamicMaterial)
}

void UP15AbilitySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(ColorParameterName, AbilityColor);
	}
}

void UP15AbilitySlotWidget::SetAbilityCost(const float NewCost) const
{
	CostText->SetText(FText::AsNumber(NewCost));
}

void UP15AbilitySlotWidget::SetCooldown(const float NewCooldown)
{
	MaxCooldown = FMath::Max(NewCooldown, 1.f);
	UpdateCooldown(NewCooldown);
}

void UP15AbilitySlotWidget::UpdateCooldown(const float NewCooldown) const
{
	CooldownText->SetText(FText::AsNumber(NewCooldown));
	DynamicMaterial->SetScalarParameterValue(AlphaParameterName, NewCooldown / MaxCooldown);
}
