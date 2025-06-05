// Copyright © 2025, Ivan Kinder

#include "UI/HUD/P16LoadScreenHUD.h"

#include "Project16.h"
#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/P16MVVMLoadScreen.h"
#include "UI/Widget/P16LoadScreenWidget.h"
#include "Util/P16Log.h"

void AP16LoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* Controller = GetOwningPlayerController())
	{
		Controller->SetInputMode(FInputModeUIOnly {});
		Controller->DisableInput(Controller);
		Controller->bShowMouseCursor = true;
	}

	IF_VALID_OR_WARN(LoadScreenViewModelClass)
	{
		LoadScreenViewModel = NewObject<UP16MVVMLoadScreen>(this, LoadScreenViewModelClass);
		LoadScreenViewModel->InitLoadSlots();
	}

	IF_VALID_OR_WARN(LoadScreenWidgetClass)
	{
		LoadScreenWidget = CreateWidget<UP16LoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
		LoadScreenWidget->AddToViewport();
		LoadScreenWidget->BP_InitSlots();
	}

	LoadScreenViewModel->LoadData();
}
