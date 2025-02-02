// Copyright © 2025, Ivan Kinder

#include "Utils/P15PrimaryWidget.h"

#include "Gameplay/Attribute/P15AttributeSet.h"
#include "Player/P15Character.h"
#include "Utils/P15GlassBallWidget.h"

void UP15PrimaryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Set ball colors.
	HealthBallWidget->GetBallDynamicMaterial()->SetVectorParameterValue(ColorParameterName, HealthColor);
	ManaBallWidget->GetBallDynamicMaterial()->SetVectorParameterValue(ColorParameterName, ManaColor);
	StrengthBallWidget->GetBallDynamicMaterial()->SetVectorParameterValue(ColorParameterName, StrengthColor);

	// Get character attributes.
	const AP15Character* OwningChar = GetOwningPlayerPawn<AP15Character>();
	EARLY_RETURN_IF(!OwningChar)
	UP15AttributeSet* AttributeSet = OwningChar->GetAttributeSet().Get();
	EARLY_RETURN_IF(!AttributeSet)

	// Subscribe to attribute changes.
	AttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedCallback);
	AttributeSet->OnManaChanged.AddUObject(this, &ThisClass::OnManaChangedCallback);
	AttributeSet->OnStrengthChanged.AddUObject(this, &ThisClass::OnStrengthChangedCallback);

	// Set initial attribute values.
	OnHealthChangedCallback(AttributeSet->GetHealthPercentage());
	OnManaChangedCallback(AttributeSet->GetManaPercentage());
	OnStrengthChangedCallback(AttributeSet->GetStrengthPercentage());
}

void UP15PrimaryWidget::OnHealthChangedCallback(const float NewHealthPercentage)
{
	UMaterialInstanceDynamic* HealthDynamicMaterial = HealthBallWidget->GetBallDynamicMaterial().Get();
	EARLY_RETURN_IF(!HealthDynamicMaterial)
	HealthDynamicMaterial->SetScalarParameterValue(LevelParameterName, NewHealthPercentage);
}

void UP15PrimaryWidget::OnManaChangedCallback(const float NewManaPercentage)
{
	UMaterialInstanceDynamic* ManaDynamicMaterial = ManaBallWidget->GetBallDynamicMaterial().Get();
	EARLY_RETURN_IF(!ManaDynamicMaterial)
	ManaDynamicMaterial->SetScalarParameterValue(LevelParameterName, NewManaPercentage);
}

void UP15PrimaryWidget::OnStrengthChangedCallback(const float NewStrengthPercentage)
{
	UMaterialInstanceDynamic* StrengthDynamicMaterial = StrengthBallWidget->GetBallDynamicMaterial().Get();
	EARLY_RETURN_IF(!StrengthDynamicMaterial)
	StrengthDynamicMaterial->SetScalarParameterValue(LevelParameterName, NewStrengthPercentage);
}
