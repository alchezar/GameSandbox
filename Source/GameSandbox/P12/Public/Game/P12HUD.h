// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P12/Public/UI/P12ReticleWidget.h"
#include "P12HUD.generated.h"

enum class EP12ReticleType : uint8;
class UP12PlayerHUDWidget;
class UP12ReticleWidget;

UCLASS()
class GAMESANDBOX_API AP12HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP12HUD();
	void ToggleReticle(const bool bShow, const EP12ReticleType ReticleType);

protected:
	virtual void BeginPlay() override;

private:
	void ShowGameScreen();

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP12PlayerHUDWidget> GameScreenClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TMap<EP12ReticleType, TSubclassOf<UP12ReticleWidget>> ReticlesMap;
	
private:
	UPROPERTY()
	UP12ReticleWidget* ReticleWidget = nullptr;
};
