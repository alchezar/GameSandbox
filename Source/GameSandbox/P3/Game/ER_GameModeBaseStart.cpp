// Copyright (C) 2023, IKinder

#include "ER_GameModeBaseStart.h"
#include "Blueprint/UserWidget.h"
#include "UI/ER_MainMenu.h"

AER_GameModeBaseStart::AER_GameModeBaseStart()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AER_GameModeBaseStart::BeginPlay()
{
	Super::BeginPlay();

	MainMenu = Cast<UER_MainMenu>(CreateWidget(GetWorld(), MainMenuWidgetClass));
	check(MainMenu);
	MainMenu->AddToViewport();
}

void AER_GameModeBaseStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FName AER_GameModeBaseStart::GetGameLevelName() const
{
	return GameLevelName;
}
