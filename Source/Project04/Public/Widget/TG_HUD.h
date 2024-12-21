// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TG_HUD.generated.h"

UCLASS()
class PROJECT04_API ATG_HUD : public AHUD
{
	GENERATED_BODY()

public:
	ATG_HUD();
	virtual void Tick(float DeltaTime) override;
	virtual void DrawHUD() override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<UUserWidget> MainWidgetClass;

private:
	UPROPERTY()
	UUserWidget* MainWidget;
};
