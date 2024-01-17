// Copyright © 2024, IKinder

#include "P13/Public/UI/P13EndGameWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UP13EndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnReleased.AddDynamic(this, &ThisClass::OnRestartButtonClicked);
	MenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMenuButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void UP13EndGameWidget::InitWidget(const bool bWin)
{
	MessageText->SetText(FText::FromString(bWin ? WinMessage : LooseMessage));	
}

void UP13EndGameWidget::OnRestartButtonClicked() 
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		const FString MapName = GetWorld()->GetMapName();
		PlayerController->ConsoleCommand("open " + MapName);
	}
}

void UP13EndGameWidget::OnMenuButtonClicked() 
{
	UGameplayStatics::OpenLevel(GetWorld(), MenuLevelName);
}

void UP13EndGameWidget::OnExitButtonClicked() 
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->ConsoleCommand("quit");
	}
}
