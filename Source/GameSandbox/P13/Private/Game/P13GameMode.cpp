// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "P13/Public/Game/P13GameState.h"

AP13GameMode::AP13GameMode()
{}

void AP13GameMode::BeginPlay()
{
	Super::BeginPlay();
}

AActor* AP13GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PlayerStarts;
	for (TActorIterator<APlayerStart> It(GetWorld(), APlayerStart::StaticClass()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		PlayerStarts.Add(PlayerStart);
	}

	auto PredicateLambda = [&](const APlayerStart* Compared) { return Compared->PlayerStartTag == *GetCurrentPhaseString(); };
	if (APlayerStart** FoundItem = PlayerStarts.FindByPredicate(PredicateLambda))
	{
		return *FoundItem;
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AP13GameMode::Respawn(AController* NewPlayer)
{
	AP13GameState* CurrentGameState = GetGameState<AP13GameState>();
	check(CurrentGameState)

	CurrentGameState->GoToNextPhase();

	AActor* BestPlayerStart = FindPlayerStart(NewPlayer, GetCurrentPhaseString());
	if (!BestPlayerStart)
	{
		RestartPlayer(NewPlayer);
	}
	RestartPlayerAtPlayerStart(NewPlayer, BestPlayerStart);
}

FString AP13GameMode::GetCurrentPhaseString() const
{
	const AP13GameState* CurrentGameState = GetGameState<AP13GameState>();
	check(CurrentGameState)

	const EP13LevelPhase LastLevelPhase = CurrentGameState->GetLastLevelPhase();
	return UEnum::GetValueAsString(LastLevelPhase).RightChop(16);
}
