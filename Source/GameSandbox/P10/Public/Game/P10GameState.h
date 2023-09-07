// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P10GameState.generated.h"

UENUM(BlueprintType)
enum class EP10WaveState : uint8
{
	WaitingToStart     UMETA(DisplayName = "Waiting for next wave"),
	InProgress         UMETA(DisplayName = "Wave in progress"),
	WaitingToComplete  UMETA(DisplayName = "Waiting to complete current wave"),
	Complete           UMETA(DisplayName = "Wave completed"),
	GameOver           UMETA(DisplayName = "Game over, character is dead")
};

UCLASS()
class GAMESANDBOX_API AP10GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnMissionComplete(APawn* InstigatorPawn, const bool bSuccess, const float ShowTime);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void SetWaveState(const EP10WaveState NewState);

protected:
	UFUNCTION()
	void OnRep_WaveState(const EP10WaveState OldState);
	void WaveStateChanged(const EP10WaveState NewState, const EP10WaveState OldState);
	
protected:
	UPROPERTY(ReplicatedUsing="OnRep_WaveState")
	EP10WaveState WaveState = EP10WaveState::WaitingToStart;
};
