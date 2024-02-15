// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P13GameModeMenu.generated.h"

UCLASS()
class GAMESANDBOX_API AP13GameModeMenu : public AGameModeBase
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameModeMenu();

protected:
	virtual void BeginPlay() override;
};
