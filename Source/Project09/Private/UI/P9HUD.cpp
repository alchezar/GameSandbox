// Copyright (C) 2023, IKinder

#include "UI/P9HUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/P9ComboWidget.h"

AP9HUD::AP9HUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP9HUD::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AP9HUD::DrawHUD()
{
	Super::DrawHUD();
}

void AP9HUD::BeginPlay()
{
	Super::BeginPlay();

	if (ComboWidgetClass)
	{
		ComboWidget = CreateWidget<UP9ComboWidget>(GetWorld(), ComboWidgetClass);
		if (!ComboWidget) return;
		ComboWidget->AddToViewport();
	}
}

void AP9HUD::UpdateComboCount(const int32 Count)
{
	if (!ComboWidget) return;
	ComboWidget->UpdateCompoCount(Count);
}

void AP9HUD::ResetCombo()
{
	if (!ComboWidget) return;
	ComboWidget->ResetCombo();
}
