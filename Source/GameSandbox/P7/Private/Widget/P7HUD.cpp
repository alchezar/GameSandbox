// Copyright (C) 2023, IKinder

#include "P7/Public/Widget/P7HUD.h"
#include "Blueprint/UserWidget.h"
#include "P7/Public/Widget/P7PlayerOverlay.h"

AP7HUD::AP7HUD() {}

void AP7HUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerOverlayClass)
	{
		PlayerOverlayWidget = CreateWidget<UP7PlayerOverlay>(GetWorld()->GetFirstPlayerController(), PlayerOverlayClass);
		if (!PlayerOverlayWidget) return;
		PlayerOverlayWidget->AddToViewport();
	}
}
