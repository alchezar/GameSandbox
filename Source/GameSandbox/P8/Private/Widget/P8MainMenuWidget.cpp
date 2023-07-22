// Copyright (C) 2023, IKinder

#include "P8/Public/Widget/P8MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "P8/Public/Game/P8GameInstance.h"

bool UP8MainMenuWidget::Initialize()
{
	return Super::Initialize();
}

void UP8MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	check(HostButton)
	check(JoinButton)
	
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostClickedHandle);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinClickedHandle);

	GameInstance = Cast<UP8GameInstance>(GetGameInstance());
}

void UP8MainMenuWidget::OnHostClickedHandle()
{
	GameInstance->Host();
}

void UP8MainMenuWidget::OnJoinClickedHandle()
{
	GameInstance->Join(AddressText->GetText().ToString());
}
