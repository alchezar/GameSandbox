// Copyright (C) 2023, IKinder

#include "P12/Public/Game/P12HUD.h"

#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Player/Controller/P12PlayerController.h"
#include "P12/Public/UI/P12HighlightInteractableWidget.h"
#include "P12/Public/UI/P12PlayerHUDWidget.h"
#include "P12/Public/UI/MainMenu/P12MainMenuWidget.h"

AP12HUD::AP12HUD()
{	
}

void AP12HUD::BeginPlay()
{
	Super::BeginPlay();
}

void AP12HUD::ShowGameScreenFor(AP12BaseCharacter* OwnerChar, const bool bShow)
{
	/* Hide (destroy) if available. */
	if (!bShow && GameScreenWidget)
	{
		GameScreenWidget->RemoveFromParent();
		GameScreenWidget = nullptr;
		return;
	}

	/* Show (create new). */
	check(GameScreenClass)
	GameScreenWidget = CreateWidget<UP12PlayerHUDWidget>(GetWorld(), GameScreenClass);
	if (!GameScreenWidget)
	{
		return;
	}
	GameScreenWidget->SetCachedCharacter(OwnerChar);
	GameScreenWidget->AddToViewport(1);

	GetCachedPlayerController()->ToggleMenuInputMode(false);
}

void AP12HUD::ShowReticle(const bool bShow, const EP12ReticleType ReticleType)
{
	/* HireReticle */
	if (!bShow && ReticleWidget)
	{
		ReticleWidget->RemoveFromParent();
		ReticleWidget = nullptr;
		return;
	}
	
	/* Remove previous reticle */
	if (ReticleWidget)
	{
		ReticleWidget->RemoveFromParent();
		ReticleWidget = nullptr;
	}
	if (!ReticlesMap[ReticleType])
	{
		return;
	}
	/* Create new reticle */
	ReticleWidget = CreateWidget<UP12ReticleWidget>(GetWorld(), ReticlesMap[ReticleType]);
	check(ReticleWidget)
	ReticleWidget->AddToViewport(0);
}

void AP12HUD::ShowMainMenu(const bool bShow)
{
	GetCachedPlayerController()->ToggleMenuInputMode(bShow);

	/* Toggle off. */
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
		return;
	}

	/* Toggle on. */
	check(MainMenuWidgetClass)
	MainMenuWidget = CreateWidget<UP12MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	check(MainMenuWidget)
	MainMenuWidget->CacheHUD(this);
	MainMenuWidget->AddToViewport(0);
	
}

void AP12HUD::ShowInteractable(const bool bShow, FName KeyName)
{
	if (HighlightWidget)
	{
		HighlightWidget->RemoveFromParent();
		HighlightWidget = nullptr;
	}
	if (!bShow)
	{
		return;
	}
	
	check(HighlightWidgetClass)
	HighlightWidget = CreateWidget<UP12HighlightInteractableWidget>(GetWorld(), HighlightWidgetClass);
	check(HighlightWidget)
	HighlightWidget->SetActionText(KeyName);
	HighlightWidget->AddToViewport(0);
}

void AP12HUD::SetInteractableText(const FName& KeyName)
{
	if (HighlightWidget)
	{
		HighlightWidget->SetActionText(KeyName);
	}
}

AP12PlayerController* AP12HUD::GetCachedPlayerController()
{
	if (!CachedController)
	{
		CachedController = Cast<AP12PlayerController>(GetOwningPlayerController());
		check(CachedController)
	}
	return CachedController;
}
