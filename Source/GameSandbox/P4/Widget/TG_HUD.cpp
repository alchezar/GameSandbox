// Copyright (C) 2023, IKinder

#include "TG_HUD.h"
#include "Blueprint/UserWidget.h"

ATG_HUD::ATG_HUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATG_HUD::BeginPlay()
{
	Super::BeginPlay();

	MainWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
	if (!MainWidget) return;

	MainWidget->AddToViewport();
}

void ATG_HUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATG_HUD::DrawHUD()
{
	Super::DrawHUD();
}

