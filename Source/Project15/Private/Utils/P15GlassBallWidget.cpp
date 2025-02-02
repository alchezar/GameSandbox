// Copyright © 2025, Ivan Kinder

#include "Utils/P15GlassBallWidget.h"

#include "Components/Image.h"

void UP15GlassBallWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BallDynamicMaterial = GlassBallImage->GetDynamicMaterial();
}

TObjectPtr<UImage> UP15GlassBallWidget::GetGlassBallImage()
{
	return GlassBallImage;
}

TObjectPtr<UMaterialInstanceDynamic> UP15GlassBallWidget::GetBallDynamicMaterial()
{
	return BallDynamicMaterial;
}
