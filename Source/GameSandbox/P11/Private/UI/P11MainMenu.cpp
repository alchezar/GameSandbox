// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "P11/Public/Game/P11GameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogP11MainMenu, All, All)

void UP11MainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
}

void UP11MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CreateBtn || !SingleBtn || !JoinBtn || !QuitBtn || !AddressBox)
	{
		UE_LOG(LogP11MainMenu, Warning, TEXT("Some button are not bind!"));
	}

	/* Middle section */
	CreateBtn->OnClicked.AddDynamic(this, &ThisClass::OnCreateHandle);
	SingleBtn->OnClicked.AddDynamic(this, &ThisClass::OnSingleHandle);
	JoinBtn->OnClicked.AddDynamic(this, &ThisClass::OnJoinHandle);
	QuitBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitHandle);

	GameMode = Cast<AP11GameModeBase>(GetWorld()->GetAuthGameMode());
	check(GameMode);
}

void UP11MainMenu::OnCreateHandle()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameMode->GetStartupMap(), true, "listen");
}

void UP11MainMenu::OnSingleHandle()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameMode->GetStartupMap());
}

void UP11MainMenu::OnJoinHandle()
{
	const FName LevelAddress = FName(AddressBox->GetText().ToString());
	UGameplayStatics::OpenLevel(GetWorld(), LevelAddress);
}

void UP11MainMenu::OnQuitHandle()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->ConsoleCommand("quit");
	}
}
