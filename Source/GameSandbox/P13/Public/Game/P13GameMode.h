// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "P13GameMode.generated.h"

UCLASS()
class GAMESANDBOX_API AP13GameMode : public AGameMode
{
	GENERATED_BODY()
	
	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameMode();

protected:
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	/* ------------------------------- This -------------------------------- */
public:
	void Respawn(AController* NewPlayer);

private:
	FString GetCurrentPhaseString() const;
	
	/* ----------------------------- Variables ----------------------------- */
};
