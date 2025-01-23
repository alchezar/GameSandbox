// Copyright (C) 2023, IKinder

#include "UI/P10GuardStateWidget.h"

#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Player/P10AIGuard.h"

void UP10GuardStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StateTextBlock->SetText(FText::FromString(" "));
}

void UP10GuardStateWidget::SetGuard(AP10AIGuard* NewGuard)
{
	if (!NewGuard) return;
	Guard = NewGuard;

	AP10AIGuard* AIGuard = Cast<AP10AIGuard>(Guard);
	if (!AIGuard) return;
	
	AIGuard->OnStatusChanged.AddUObject(this, &ThisClass::OnStateChangesHandle);
	OnStateChangesHandle(AIGuard->GetGuardState());
}

void UP10GuardStateWidget::OnStateChangesHandle(EP10AIGuardState GuardState)
{
	FString StateString;	
	switch (GuardState)
	{
	case EP10AIGuardState::Idle :       StateString = " "; break;
	case EP10AIGuardState::Suspicious : StateString = "?"; break;
	case EP10AIGuardState::Alert :      StateString = "!"; break;
	default:							StateString = ""; 
	}
	
	FColor StateColor;
	switch (GuardState)
	{
	case EP10AIGuardState::Idle :       StateColor = FColor::White; break;
	case EP10AIGuardState::Suspicious : StateColor = FColor::Orange; break;
	case EP10AIGuardState::Alert :      StateColor = FColor::Red;    break;
	default:							StateColor = FColor::White;
	}
	
	StateTextBlock->SetText(FText::FromString(StateString));
	StateTextBlock->SetColorAndOpacity(FSlateColor(StateColor));
}
