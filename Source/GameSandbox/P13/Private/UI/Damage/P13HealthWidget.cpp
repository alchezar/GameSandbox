// Copyright © 2024, IKinder

#include "P13/Public/UI/Damage/P13HealthWidget.h"

#include "Components/ProgressBar.h"
#include "P13/Public/Component/Actor/P13CharacterAttributesComponent.h"

void UP13HealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APawn* OwningPlayerPawn = GetOwningPlayerPawn();
	if (!OwningPlayerPawn)
	{
		return;
	}
	UP13CharacterAttributesComponent* AttributesComponent = OwningPlayerPawn->FindComponentByClass<UP13CharacterAttributesComponent>();
	if (!AttributesComponent)
	{
		return;
	}
	AttributesComponent->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedHandle);
	AttributesComponent->OnHealthOver.AddUObject(this, &ThisClass::OnDeathHandle);
}

void UP13HealthWidget::OnHealthChangedHandle(const float NewHealth, const float LastDamage, const float HealthAlpha) const
{
	HealthBar->SetPercent(HealthAlpha);
}

void UP13HealthWidget::OnDeathHandle()
{
	RemoveFromParent();
}
