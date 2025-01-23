// Copyright (C) 2023, IKinder

#include "UI/P11MenuFPS.h"

#include "Components/TextBlock.h"

void UP11MenuFPS::NativeConstruct()
{
	Super::NativeConstruct();
	OnVisibilityChanged.AddDynamic(this, &ThisClass::OnVisibilityChangedHandle);
	
	GetWorld()->GetTimerManager().SetTimer(FpsTimer, this, &ThisClass::ShowFpsEverySecond, 1.f, true);
}

void UP11MenuFPS::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UP11MenuFPS::ShowFpsEverySecond() const
{
	const int32 FpsInt = FMath::TruncToInt(1.f / GetWorld()->GetDeltaSeconds());
	FString FpsString =  "FPS: ";
	FpsString += FpsInt < 100 ? "0" : "";
	FpsString += FString::FromInt(FpsInt);
	FpsText->SetText(FText::FromString(FpsString));
}

void UP11MenuFPS::OnVisibilityChangedHandle(ESlateVisibility InVisibility)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (InVisibility != ESlateVisibility::Visible)
	{
		TimerManager.PauseTimer(FpsTimer);
		return;
	}
	if (TimerManager.IsTimerActive(FpsTimer))
	{
		return;
	}
	TimerManager.UnPauseTimer(FpsTimer);
}
