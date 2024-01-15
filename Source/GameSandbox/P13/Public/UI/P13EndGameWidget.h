// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13EndGameWidget.generated.h"

class UButton;

UCLASS()
class GAMESANDBOX_API UP13EndGameWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnRestartButtonClicked();
	UFUNCTION()
	void OnMenuButtonClicked();
	UFUNCTION()
	void OnExitButtonClicked();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
	UButton* MenuButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(EditAnywhere, Category = "C++")	
	FName MenuLevelName;
};
