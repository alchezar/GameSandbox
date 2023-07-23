// Copyright (C) 2023, IKinder

#include "P8/Public/Widget/P8MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

bool UP8MainMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UP8MainMenuWidget::Setup()
{
	AddToViewport();
	MenuInterface = Cast<IP8MenuInterface>(GetGameInstance());
}

void UP8MainMenuWidget::SetMenuInterface(IP8MenuInterface* NewMenuInterface)
{
	MenuInterface = NewMenuInterface;
}

void UP8MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	check(HostButton)
	check(JoinButton)
	
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostClickedHandle);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinClickedHandle);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClickedHandle);
}

void UP8MainMenuWidget::OnHostClickedHandle()
{
	if (!MenuInterface) return;
	MenuInterface->Host();
}

void UP8MainMenuWidget::OnJoinClickedHandle()
{
	if (!MenuInterface || AddressText->GetText().IsEmpty()) return;
	MenuInterface->Join(AddressText->GetText().ToString());
}

void UP8MainMenuWidget::OnQuitClickedHandle()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	PC->ConsoleCommand("quit");
}
