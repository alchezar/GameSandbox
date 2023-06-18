// Copyright (C) 2023, IKinder

#include "ARPauseMenu.h"
#include "ARMenuButton.h"
#include "Kismet/GameplayStatics.h"
#include "P6/Player/ARPlayerController.h"

void UARPauseMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ToMainButton->OnButtonClickedDelegate.AddDynamic(this, &ThisClass::OnReturnToMainClicked);
	ToGameButton->OnButtonClickedDelegate.AddDynamic(this, &ThisClass::OnReturnToGameClicked);
}

void UARPauseMenu::OnReturnToMainClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainLevelName);
}

void UARPauseMenu::OnReturnToGameClicked()
{
	AARPlayerController* Controller = Cast<AARPlayerController>(GetOwningPlayer());
	if (!Controller) return;

	Controller->TogglePauseMenu();

}
