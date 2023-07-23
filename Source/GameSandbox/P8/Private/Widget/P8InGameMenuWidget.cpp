// Copyright (C) 2023, IKinder

#include "P8/Public/Widget/P8InGameMenuWidget.h"
#include "Components/Button.h"
#include "P8/Public/Interface/P8PlayerControllerInterface.h"

bool UP8InGameMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UP8InGameMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CancelButton->OnClicked.AddDynamic(this, &ThisClass::OnCancelClicked);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClicked);
}

void UP8InGameMenuWidget::SetPCInterface(IP8PlayerControllerInterface* NewPCInterface)
{
	PCInterface = NewPCInterface;
}


void UP8InGameMenuWidget::OnCancelClicked()
{
	if (!PCInterface) return;
	PCInterface->UnPause();
}

void UP8InGameMenuWidget::OnQuitClicked()
{
	if (!PCInterface) return;
	PCInterface->GoToLobby();
}
