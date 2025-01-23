// Copyright (C) 2023, IKinder

#include "Player/P10PlayerController.h"

#include "EngineUtils.h"
#include "UI/P10HUD.h"

void AP10PlayerController::OnMissionCompleted(APawn* InstigatorPawn, const bool bSuccess, const float ShowTime)
{
	if (AP10HUD* CurrentHUD = Cast<AP10HUD>(GetHUD()))
	{
		CurrentHUD->OnMissionCompleted(bSuccess, ShowTime);
	}
	check(ViewPointClass)
	if (AActor* TargetActor = *TActorIterator(GetWorld(), ViewPointClass))
	{
		SetViewTargetWithBlend(TargetActor, 0.5f, VTBlend_Cubic);
	}
}
