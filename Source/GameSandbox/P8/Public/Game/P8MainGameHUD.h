// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P8MainGameHUD.generated.h"

class UP8InGameMenuWidget;

UCLASS()
class GAMESANDBOX_API AP8MainGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AP8MainGameHUD();
	virtual void Tick(float DeltaTime) override;
	UP8InGameMenuWidget* ShowPauseMenu(const bool bVisibility);
	FORCEINLINE UP8InGameMenuWidget* GetPauseMenuWidget() const { return PauseMenuWidget; };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP8InGameMenuWidget> PauseMenuClass;

private:
	UPROPERTY()
	UP8InGameMenuWidget* PauseMenuWidget;
};
