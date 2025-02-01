// Copyright © 2025, Ivan Kinder

#include "Player/P15HUD.h"

#include "Project15.h"
#include "Blueprint/UserWidget.h"
#include "Utils/P15PrimaryWidget.h"

void AP15HUD::BeginPlay()
{
	Super::BeginPlay();

	PrimaryWidget = CreateWidget<UP15PrimaryWidget>(GetOwningPlayerController(), PrimaryWidgetClass);
	EARLY_RETURN_IF(!PrimaryWidget)
	PrimaryWidget->AddToViewport(0);
}
