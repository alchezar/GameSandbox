// Copyright (C) 2023, IKinder

#include "ARMainMenu.h"
#include "ARMenuButton.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UARMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ButtonHost->OnButtonClickedDelegate.AddDynamic(this, &ThisClass::OnHostButtonClicked);
	ButtonJoin->OnButtonClickedDelegate.AddDynamic(this, &ThisClass::OnJoinButtonClicked);
	ButtonQuit->OnButtonClickedDelegate.AddDynamic(this, &ThisClass::OnQuitButtonClicked);
}

void UARMainMenu::OnHostButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, true, "listen");
}

void UARMainMenu::OnJoinButtonClicked()
{
	const FString IpString = IpFieldTextBox->GetText().ToString();
	if (IpString.Len() == 0) return;

	UGameplayStatics::OpenLevel(GetWorld(), FName(IpString));
}

void UARMainMenu::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, true);
}
