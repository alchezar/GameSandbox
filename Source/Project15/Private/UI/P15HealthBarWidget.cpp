// Copyright © 2025, Ivan Kinder

#include "UI/P15HealthBarWidget.h"

#include "Components/ProgressBar.h"
#include "Gameplay/Attribute/P15AttributeSet.h"
#include "Player/P15Character.h"

void UP15HealthBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UP15HealthBarWidget::FindOwner(AP15Character* CurrentOwner)
{
	EARLY_RETURN_IF(!CurrentOwner)
	const TObjectPtr<UP15AttributeSet> AttributeSet = CurrentOwner->GetAttributeSet();
	EARLY_RETURN_IF(!AttributeSet)

	AttributeSet->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedCallback);
	OnHealthChangedCallback(AttributeSet->GetHealthPercentage());
}

void UP15HealthBarWidget::OnHealthChangedCallback(const float NewHealthPercentage)
{
	HealthBar->SetPercent(NewHealthPercentage);
}
