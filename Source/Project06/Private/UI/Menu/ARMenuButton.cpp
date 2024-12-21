// Copyright (C) 2023, IKinder

#include "UI/Menu/ARMenuButton.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UARMenuButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button->OnClicked.AddDynamic(this, &ThisClass::OnMenuButtonClicked);
	ButtonText->SetText(Text);
}

void UARMenuButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UARMenuButton::OnMenuButtonClicked()
{
	OnButtonClickedDelegate.Broadcast();
}
