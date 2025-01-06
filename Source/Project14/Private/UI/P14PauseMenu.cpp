// Copyright © 2024, IKinder

#include "UI/P14PauseMenu.h"

#include "Components/Button.h"
#include "Player/P14Controller.h"

void UP14PauseMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnReleased.AddDynamic(this, &ThisClass::OnCloseButtonCallback);
}

void UP14PauseMenu::OnCloseButtonCallback()
{
	if (AP14Controller* PlayerController = GetOwningPlayer<AP14Controller>())
	{
		PlayerController->TogglePause();
	}
}
