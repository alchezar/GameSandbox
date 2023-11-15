// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P12/Public/UI/P12ReticleWidget.h"
#include "P12HUD.generated.h"

class AP12PlayerController;
class UP12MainMenuWidget;
enum class EP12ReticleType : uint8;
class UP12PlayerHUDWidget;
class UP12ReticleWidget;

UCLASS()
class GAMESANDBOX_API AP12HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP12HUD();
	void ShowReticle(const bool bShow, const EP12ReticleType ReticleType);
	void ShowGameScreenFor(AP12BaseCharacter* OwnerChar, const bool bShow = true);
	void ShowMainMenu(const bool bShow);

protected:
	virtual void BeginPlay() override;

private:
	AP12PlayerController* GetCachedPlayerController();

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP12PlayerHUDWidget> GameScreenClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP12MainMenuWidget> MainMenuWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TMap<EP12ReticleType, TSubclassOf<UP12ReticleWidget>> ReticlesMap;
	
private:
	UPROPERTY()
	AP12PlayerController* CachedController = nullptr;
	UPROPERTY()
	UP12ReticleWidget* ReticleWidget = nullptr;
	UPROPERTY()
	UP12MainMenuWidget* MainMenuWidget = nullptr;
	UPROPERTY()
	UP12PlayerHUDWidget* GameScreenWidget = nullptr;
};
