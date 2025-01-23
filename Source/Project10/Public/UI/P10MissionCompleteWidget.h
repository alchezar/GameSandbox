// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P10MissionCompleteWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECT10_API UP10MissionCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMissionText(const FString& NewText) const;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MissionText;
};
