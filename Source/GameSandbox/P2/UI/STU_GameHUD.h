// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STU_GameHUD.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_GameHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASTU_GameHUD();
	virtual void DrawHUD() override;

private:
	void DrawCrossHair();
};
