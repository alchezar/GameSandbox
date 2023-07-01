// Copyright (C) 2023, IKinder

#include "P7/Public/Widget/Component/P7HealthBarComponent.h"
#include "P7/Public/Widget/P7HealthBarWidget.h"

UP7HealthBarComponent::UP7HealthBarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP7HealthBarComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP7HealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP7HealthBarComponent::SetHealthPercent(const float HealthPercent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UP7HealthBarWidget>(GetUserWidgetObject());
	}
	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(HealthPercent);
	}
}
