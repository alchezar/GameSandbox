// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Game/P8GameMode.h"
#include "P8GameModeLobby.generated.h"

UCLASS()
class PROJECT08_API AP8GameModeLobby : public AP8GameMode
{
	GENERATED_BODY()

public:
	AP8GameModeLobby();
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

protected:
	virtual void BeginPlay() override;

private:
	void TravelToGame() const;

private:
	uint32 NumberOfPlayers = 0;
	FTimerHandle GameStartTimer;
};
