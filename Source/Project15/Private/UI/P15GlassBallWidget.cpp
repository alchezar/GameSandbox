// Copyright © 2025, Ivan Kinder

#include "UI/P15GlassBallWidget.h"

#include "Components/Image.h"

void UP15GlassBallWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BallDynamicMaterial = GlassBallImage->GetDynamicMaterial();
	check(BallDynamicMaterial)
}

void UP15GlassBallWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set ball colors.
	if (BallDynamicMaterial)
	{
		BallDynamicMaterial->SetVectorParameterValue(ColorParameterName, BallColor);
	}
}

TObjectPtr<UImage> UP15GlassBallWidget::GetGlassBallImage()
{
	return GlassBallImage;
}

TObjectPtr<UMaterialInstanceDynamic> UP15GlassBallWidget::GetBallDynamicMaterial()
{
	return BallDynamicMaterial;
}
