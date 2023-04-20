// Copyright (C) 2023, IKinder

#include "STU_PlayerController.h"
#include "Component/STU_RespawnComponent.h"

ASTU_PlayerController::ASTU_PlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTU_RespawnComponent>("RespawnComponent");
}

// void ASTU_PlayerController::OnPossess(APawn* InPawn)
// {
// 	Super::OnPossess(InPawn);
//
// 	OnNewPawn.Broadcast(InPawn);
// }
