// Copyright (C) 2023, IKinder

#include "UI/ER_MainMenu.h"

#include "Components/Button.h"
#include "Game/ER_GameModeBaseStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UER_MainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!StartBtn || !QuitBtn) return;

	StartBtn->OnClicked.AddDynamic(this, &ThisClass::OnStartClicked);
	QuitBtn->OnClicked.AddDynamic(this, &ThisClass::OnQuitClicked);

	GameMode = Cast<AER_GameModeBaseStart>(GetWorld()->GetAuthGameMode());
	check(GameMode);
}

void UER_MainMenu::OnStartClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), GameMode->GetGameLevelName());
	RemoveFromParent();
}

void UER_MainMenu::OnQuitClicked()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
}
