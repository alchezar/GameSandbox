// Copyright (C) 2023, IKinder

#include "ER_GameHud.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "P3/Game/ER_GameModeBase.h"

void UER_GameHud::NativeConstruct()
{
	Super::NativeConstruct();

	PauseBtn->OnClicked.AddDynamic(this, &ThisClass::OnPauseClicked);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, this);
}

void UER_GameHud::InitializeHud(AER_GameModeBase* GameMode)
{
	if (!GameMode) return;

	CoinsCount->SetText(FText::AsNumber(0));
	GameMode->OnCoinsCountChanged.AddUObject(this, &ThisClass::SetCoinsCount);
	GameMode->OnLivesCountChanged.AddUObject(this, &ThisClass::SetLivesCount);
}

void UER_GameHud::SetCoinsCount(const int32 NewCount)
{
	CoinsCount->SetText(FText::AsNumber(NewCount));
}

void UER_GameHud::SetLivesCount(const int32 NewCount)
{
	LivesCount->SetText(FText::AsNumber(NewCount));
}

void UER_GameHud::OnPauseClicked()
{
	if (!PauseWidgetClass) return;

	UGameplayStatics::SetGamePaused(GetWorld(), true);
	if (UUserWidget* PauseWidget = CreateWidget(GetWorld(), PauseWidgetClass))
	{
		PauseWidget->AddToViewport();
	}
}
