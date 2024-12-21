// Copyright (C) 2023, IKinder

#include "Project01/Public/Widget/PlayerHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Project01/Public/Character/BatteryCollectorGameMode.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

UProgressBar* UPlayerHUD::GetPowerBar() const
{
	return PowerBar;
}

void UPlayerHUD::UpdatePowerBar(const float Percent) const
{
	PowerBar->SetPercent(Percent);
}

void UPlayerHUD::UpdateTextBlock(const EPickupGameState CurrentState) const
{
	switch (CurrentState)
	{
	case EPickupGameState::Win:
		TextBlockPower->SetText(FText::FromString("You WIN!!!"));
		TextBlockPower->SetColorAndOpacity(FSlateColor(FLinearColor(0.2f, 1.f, 0.2f)));
		break;
	case EPickupGameState::Lost:
		TextBlockPower->SetText(FText::FromString("You LOSE!"));
		TextBlockPower->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 0.2f, 0.2f)));

		break;
	case EPickupGameState::None:    //fallthrough
	case EPickupGameState::Playing: //fallthrough
	default:
		TextBlockPower->SetText(FText::GetEmpty());;
	}
}
