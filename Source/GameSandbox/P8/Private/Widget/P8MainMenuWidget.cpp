// Copyright (C) 2023, IKinder

#include "P8/Public/Widget/P8MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "P8/Public/Widget/P8ServerRow.h"

DEFINE_LOG_CATEGORY_STATIC(LogP8MainMenuWidget, All, All)

bool UP8MainMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UP8MainMenuWidget::Setup()
{
	AddToViewport();
	MenuInterface = Cast<IP8MenuInterface>(GetGameInstance());
}

void UP8MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	check(HostButton)
	check(JoinButton)
	check(JoinWidgetSwitcher)
	check(CancelButton)
	check(ConnectButton)
	check(ServerListScrollBox)
	check(ServerRowClass)
	
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostClickedHandle);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinClickedHandle);
	QuitButton->OnClicked.AddDynamic(this, &ThisClass::OnQuitClickedHandle);
	CancelButton->OnClicked.AddDynamic(this, &ThisClass::OnCancelClickedHandle);
	ConnectButton->OnClicked.AddDynamic(this, &ThisClass::OnConnectClickedHandle);

	JoinWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UP8MainMenuWidget::OnHostClickedHandle()
{
	if (!MenuInterface) return;
	MenuInterface->Host();
}

void UP8MainMenuWidget::OnJoinClickedHandle()
{
	JoinWidgetSwitcher->SetActiveWidgetIndex(1);
	
	if (!MenuInterface) return;
	MenuInterface->RefreshServerList();
}

void UP8MainMenuWidget::OnQuitClickedHandle()
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	PC->ConsoleCommand("quit");
}

void UP8MainMenuWidget::OnCancelClickedHandle()
{
	JoinWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UP8MainMenuWidget::OnConnectClickedHandle()
{
	if (SelectedIndex.IsSet())
	{
		UE_LOG(LogP8MainMenuWidget, Log, TEXT("Selected index: %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogP8MainMenuWidget, Log, TEXT("Selected index NOT set!"));
	}
	
	if (MenuInterface)
	{
		MenuInterface->Join("");
	}
}

void UP8MainMenuWidget::SetServerList(TArray<FString> ServerNames)
{
	ServerListScrollBox->ClearChildren();
	uint32 Index = 0;
	for (const FString& ServerName : ServerNames)
	{
		UP8ServerRow* ServerRowWidget = CreateWidget<UP8ServerRow>(GetWorld(), ServerRowClass);
		if (!ServerRowWidget) return;
		ServerRowWidget->Setup(this, Index++);
		ServerRowWidget->SetServerName( FText::FromString(ServerName));
		ServerListScrollBox->AddChild(ServerRowWidget);
	}
	
}

void UP8MainMenuWidget::SetSelectedIndex(const uint32 Index)
{
	SelectedIndex = Index;
}
