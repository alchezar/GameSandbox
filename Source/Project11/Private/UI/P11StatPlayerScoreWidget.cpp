// Copyright (C) 2023, IKinder

#include "UI/P11StatPlayerScoreWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Player/P11PlayerState.h"

void UP11StatPlayerScoreWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &ThisClass::UpdateScore, 1.f, true, 0.f);
}

void UP11StatPlayerScoreWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(UpdateTimer);
}

void UP11StatPlayerScoreWidget::SetLocalStyle() const
{
	if (!PlayerState)
	{
		return;
	}
	const APlayerController* PlayerController = PlayerState->GetPlayerController();
	if (PlayerController && PlayerController->IsLocalController())
	{
		RowBorder->Background.OutlineSettings.Width = 5.f;
		const FLinearColor FgColor = RowBorder->GetBrushColor();
		const FLinearColor BgColor = RowBorder->Background.OutlineSettings.Color.GetSpecifiedColor();
		RowBorder->SetBrushColor(BgColor);
		RowBorder->Background.OutlineSettings.Color = FSlateColor(FgColor);
		PlayerNameText->SetColorAndOpacity(FgColor);
		KillsText->SetColorAndOpacity(FgColor);
		DeathText->SetColorAndOpacity(FgColor);
		PingText->SetColorAndOpacity(FgColor);
	}
}

void UP11StatPlayerScoreWidget::SetPlayerState(AP11PlayerState* CurrentState)
{
	PlayerState = CurrentState;
	SetLocalStyle();
}

void UP11StatPlayerScoreWidget::UpdateScore() const
{
	if (!PlayerState)
	{
		return;
	}
	PlayerNameText->SetText(PlayerState->GetThePlayerName());
	DeadImg->SetColorAndOpacity(PlayerState->GetIsDead() ? FLinearColor::White : FLinearColor(1.f, 1.f, 1.f, 0.f));
	KillsText->SetText(FText::AsNumber(PlayerState->GetKills()));
	DeathText->SetText(FText::AsNumber(PlayerState->GetDeaths()));
	PingText->SetText(FText::AsNumber(FMath::TruncToInt(PlayerState->GetPingInMilliseconds())));
}
