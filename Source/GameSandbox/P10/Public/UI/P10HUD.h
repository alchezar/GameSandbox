// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P10HUD.generated.h"

UCLASS()
class GAMESANDBOX_API AP10HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP10HUD();
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	UTexture2D* CrosshairTex;
};
