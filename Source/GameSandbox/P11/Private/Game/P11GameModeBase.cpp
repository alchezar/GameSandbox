// Copyright (C) 2023, IKinder

#include "P11/Public/Game/P11GameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "P11/Public/Player/P11Character.h"
#include "P11/Public/Player/P11PlayerController.h"
#include "P11/Public/Player/P11PlayerState.h"
#include "P11/Public/UI/P11HUD.h"

AP11GameModeBase::AP11GameModeBase()
{
	DefaultPawnClass = AP11Character::StaticClass();
	HUDClass = AP11HUD::StaticClass();
	PlayerControllerClass = AP11PlayerController::StaticClass();
	PlayerStateClass = AP11PlayerState::StaticClass();
}

void AP11GameModeBase::Respawn(AController* Controller)
{
	/* Destroy dead character mesh. */
	if (APawn* DeadPawn = Controller->GetPawn())
	{
		Controller->UnPossess();
		DeadPawn->SetLifeSpan(5.f);
	}

	/* Find random player start transform. */
	TArray<APlayerStart*> AllPlayerStarts;
	for(TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		AllPlayerStarts.Add(PlayerStart);
	}
	const int32 RandPlayerStart = FMath::RandRange(0, AllPlayerStarts.Num() - 1);
	const FTransform SpawnTransform = AllPlayerStarts[RandPlayerStart]->GetActorTransform();

	/* Spawn new character. */
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (AP11Character* NewChar = GetWorld()->SpawnActor<AP11Character>(RespawnCharClass, SpawnTransform, Params))
	{
		Controller->Possess(NewChar);
	}
}
