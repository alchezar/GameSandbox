// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P11PlayerState.generated.h"

UCLASS()
class GAMESANDBOX_API AP11PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	void AddKill();
	void AddDeath();

	FORCEINLINE FText GetThePlayerName() const { return PlayerName; }
	FORCEINLINE bool GetIsDead() const { return bDead; }
	FORCEINLINE int32 GetKills() const { return Kills; }
	FORCEINLINE int32 GetDeaths() const { return Deaths; }
	FORCEINLINE void SetIsDead(const bool bNewDead) { bDead = bNewDead; }

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void Server_SendNameToSrv(const FText& Name);

protected:
	UPROPERTY(Replicated)
	FText PlayerName;
	UPROPERTY(Replicated)
	bool bDead;
	UPROPERTY(Replicated)
	int32 Kills;
	UPROPERTY(Replicated)
	int32 Deaths;
};
