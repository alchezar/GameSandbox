// Copyright © 2024, IKinder

#include "P13/Public/UI/P13EndGameWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UP13EndGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnReleased.AddDynamic(this, &ThisClass::OnRestartButtonClicked);
	MenuButton->OnClicked.AddDynamic(this, &ThisClass::OnMenuButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void UP13EndGameWidget::OnRestartButtonClicked() 
{
	// const FString LevelName = GetWorld()->GetCurrentLevel()->GetName();
	// UGameplayStatics::OpenLevel(GetWorld(), *LevelName);

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
