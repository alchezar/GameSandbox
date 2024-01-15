// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "P13PlayerState.generated.h"

UCLASS()
class GAMESANDBOX_API AP13PlayerState : public APlayerState
{
	GENERATED_BODY()
	
	/* ------------------------------- Super ------------------------------- */
public:
	AP13PlayerState();

protected:
	virtual void BeginPlay() override;
	
	/* ------------------------------- This -------------------------------- */
public:
	bool GetPlayerCanRespawnAfterDeath();
	bool GetPlayerCanRespawn() const;
	void OnPlayerDied();
	void ResetLives();
	
	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	int32 MaxLives = 3;

private:
	int32 CurrentLives = 0;
};
