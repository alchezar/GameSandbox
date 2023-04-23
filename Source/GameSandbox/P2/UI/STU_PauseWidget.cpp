// Copyright (C) 2023, IKinder

#include "STU_PauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameMode.h"

void  USTU_PauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &ThisClass::OnClearPause);
	}
}

void USTU_PauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}
