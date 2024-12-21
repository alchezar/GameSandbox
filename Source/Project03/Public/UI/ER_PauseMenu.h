// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ER_PauseMenu.generated.h"

class UButton;
class AER_GameModeBase;

UCLASS()
class PROJECT03_API UER_PauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnContinueClicked();
	UFUNCTION()
	void OnMainClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* ContinueBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* MainBtn;

private:
	UPROPERTY()
	AER_GameModeBase* GameMode;
};
