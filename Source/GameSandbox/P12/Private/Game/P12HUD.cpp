// Copyright (C) 2023, IKinder

#include "P12/Public/Game/P12HUD.h"

#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/UI/P12PlayerHUDWidget.h"

AP12HUD::AP12HUD()
{	
}

void AP12HUD::BeginPlay()
{
	Super::BeginPlay();
	// ShowGameScreen();
}

void AP12HUD::ShowGameScreenFor(AP12BaseCharacter* OwnerChar)
{
	if (!GameScreenClass)
	{
		return;
	}
	UP12PlayerHUDWidget* GameScreenWidget = CreateWidget<UP12PlayerHUDWidget>(GetWorld(), GameScreenClass);
	if (!GameScreenWidget)
	{
		return;
	}
	GameScreenWidget->SetCachedCharacter(OwnerChar);
	GameScreenWidget->AddToViewport(1);
}

void AP12HUD::ToggleReticle(const bool bShow, const EP12ReticleType ReticleType)
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
