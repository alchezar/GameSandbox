// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DebugCameraHUD.h"
#include "P8MainMenuHUD.generated.h"

class UP8MainMenuWidget;

UCLASS()
class GAMESANDBOX_API AP8MainMenuHUD : public ADebugCameraHUD
{
	GENERATED_BODY()

public:
	AP8MainMenuHUD();
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE UP8MainMenuWidget* GetMainMenuWidget() const { return MainMenuWidget; };

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Widget")
	TSubclassOf<UP8MainMenuWidget> MainMenuWidgetClass;

private:
	UPROPERTY()
	UP8MainMenuWidget* MainMenuWidget;
};
