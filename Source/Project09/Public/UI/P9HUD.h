// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "P9HUD.generated.h"

class UP9ComboWidget;

UCLASS()
class PROJECT09_API AP9HUD : public AHUD
{
	GENERATED_BODY()

public:
	AP9HUD();
	virtual void Tick(const float DeltaSeconds) override;
	virtual void DrawHUD() override;
	void UpdateComboCount(const int32 Count);
	void ResetCombo();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP9ComboWidget> ComboWidgetClass;

private:
	UPROPERTY()
	UP9ComboWidget* ComboWidget;
};
