// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Game/STU_CoreTypes.h"
#include "GameFramework/HUD.h"
#include "STU_GameHUD.generated.h"

class USTU_BaseWidget;

UCLASS()
class PROJECT02_API ASTU_GameHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASTU_GameHUD();
	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
	void OnMatchStateChangedHandle(ESTU_MatchState NewState);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

private:
	UPROPERTY()
	TMap<ESTU_MatchState, USTU_BaseWidget*> GameWidgets;
	UPROPERTY()
	USTU_BaseWidget* CurrentWidget = nullptr;
};
