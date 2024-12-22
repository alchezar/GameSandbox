// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STU_Menu_GoToMain.generated.h"

class UButton;

UCLASS()
class GAMESANDBOX_API USTU_Menu_GoToMain : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnMainMenu();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;
};
