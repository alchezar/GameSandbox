// Copyright © 2025, Ivan Kinder

#include "Utils/P15GlassBallWidget.h"

#include "Components/Image.h"
#include "Gameplay/Attribute/P15AttributeSet.h"
#include "Player/P15Character.h"

void UP15GlassBallWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	DynamicHealthMaterial = GlassBallImage->GetDynamicMaterial();
	if (const AP15Character* OwningChar = GetOwningPlayerPawn<AP15Character>())
	{
		OwningChar->GetAttributeSet()->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedCallback);
	}
	OnHealthChangedCallback();
}

void UP15GlassBallWidget::OnHealthChangedCallback(const float NewHealthPercentage)
{
	EARLY_RETURN_IF(!DynamicHealthMaterial)
	DynamicHealthMaterial->SetScalarParameterValue(HealthParameterName, NewHealthPercentage);
}
