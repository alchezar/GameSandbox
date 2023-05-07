// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ER_PauseMenu.generated.h"

class UButton;

UCLASS()
class GAMESANDBOX_API UER_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnContinueClicked();
	UFUNCTION()
	void OnRestartClicked();
	UFUNCTION()
	void OnExitClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ContinueBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* RestartBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitBtn;
		
};
