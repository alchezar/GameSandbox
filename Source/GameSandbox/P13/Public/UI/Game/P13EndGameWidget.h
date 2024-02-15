// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13EndGameWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class GAMESANDBOX_API UP13EndGameWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	void InitWidget(const bool bWin);

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
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;

	UPROPERTY(EditAnywhere, Category = "C++")
	FName MenuLevelName;
	UPROPERTY(EditAnywhere, Category = "C++")
	FString WinMessage = "You WIN!!!";
	UPROPERTY(EditAnywhere, Category = "C++")
	FString LooseMessage = "You LOOSE!";
};
