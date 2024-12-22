// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STU_HealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class GAMESANDBOX_API USTU_HealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthPercent(float Percent) const;

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar", meta = (Units = "%"))
	float VisibilityThreshold = 0.8f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar", meta = (Units = "%"))
	float ColorThreshold = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar")
	FLinearColor GoodColor = FLinearColor(0.0f, 0.5f, 0.2f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder | HealthBar")
	FLinearColor BadColor = FLinearColor(1.f, 0.1f, 0.1f);
};
