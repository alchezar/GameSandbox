// Copyright (C) 2023, IKinder

#include "TG_WidgetMain.h"
#include "Components/TextBlock.h"
#include "P4/Game/TG_GameMode.h"
#include "P4/Player/TG_FirstPersonCharacter.h"

bool UTG_WidgetMain::Initialize()
{
	return Super::Initialize();
}

void UTG_WidgetMain::BindDelegates(ATG_GameMode* TheGameMode, ATG_FirstPersonCharacter* ThePlayer)
{
	if (TheGameMode)
	{
		TheGameMode->OnScoreChanged.AddUObject(this, &ThisClass::OnScoreChangeHandle);
	}
	if (ThePlayer)
	{
		ThePlayer->OnHealthChanged.AddUObject(this, &ThisClass::OnHeathChangedHandle);
	}
}

void UTG_WidgetMain::SetHealth(const float NewHealth)
{
	HealthText->SetText(FText::AsNumber(NewHealth));
}

void UTG_WidgetMain::SetScore(const int32 NewScore)
{
	ScoreText->SetText(FText::AsNumber(NewScore + 1));
}

void UTG_WidgetMain::OnHeathChangedHandle(float NewHealth)
{
	SetHealth(NewHealth);
}

void UTG_WidgetMain::OnScoreChangeHandle(int32 NewScore)
{
	SetScore(NewScore);
}
