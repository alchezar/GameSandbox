// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameState.h"

#include "Engine/PawnIterator.h"
#include "P10/Public/Player/P10PlayerController.h"

void AP10GameState::Multicast_OnMissionComplete_Implementation(APawn* InstigatorPawn, bool bSuccess)
{
	for (auto It = TActorIterator<APlayerController>(GetWorld(), APlayerController::StaticClass()); It; ++It)
	{
		AP10PlayerController* Controller = Cast<AP10PlayerController>(*It);
		if (!Controller || !Controller->IsLocalController()) continue;
		APawn* Pawn = Controller->GetPawn();
		if (!Pawn || !Pawn->IsLocallyControlled()) continue;

		Pawn->DisableInput(Controller);
		Controller->OnMissionCompleted(InstigatorPawn, bSuccess);
	}
}
