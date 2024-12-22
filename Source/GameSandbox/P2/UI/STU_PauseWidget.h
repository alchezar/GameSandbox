// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWidget.h"
#include "STU_PauseWidget.generated.h"

class UButton;

UCLASS()
class GAMESANDBOX_API USTU_PauseWidget : public USTU_BaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;
};
