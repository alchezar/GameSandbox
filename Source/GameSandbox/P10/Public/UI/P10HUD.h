// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P10HUD.generated.h"

class UTexture2D;

UCLASS()
class GAMESANDBOX_API AP10HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP10HUD();
	virtual void DrawHUD() override;
	void OnMissionCompleted(bool bSuccess);

protected:
	virtual void BeginPlay() override;

private:
	void DrawCrosshair(const bool bDraw);
	void DrawHealthIndicator();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	UTexture2D* CrosshairTex;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UUserWidget> HealthIndicatorWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UUserWidget> MissionCompletedWidgetClass;

private:
	UPROPERTY()
	UUserWidget* CrosshairWidget;
	UPROPERTY()
	UUserWidget* HealthIndicator;
};
