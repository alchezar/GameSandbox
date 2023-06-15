// Copyright (C) 2023, IKinder

#include "ARPlayerController.h"
#include "Blueprint/UserWidget.h"

AARPlayerController::AARPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPlayerController::BeginPlay()
{
	Super::BeginPlay();
	AddWidget();
}

void AARPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPlayerController::AddWidget()
{
	if (!HUDClass) return;
	
	HUDWidget = CreateWidget(GetWorld(), HUDClass);
	HUDWidget->AddToViewport();
}

