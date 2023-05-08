// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ER_MainMenu.generated.h"

class UButton;
class AER_GameModeBaseStart;

UCLASS()
class GAMESANDBOX_API UER_MainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnStartClicked();
	UFUNCTION()
	void OnQuitClicked();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* StartBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* QuitBtn;

private:
	UPROPERTY()
	AER_GameModeBaseStart* GameMode;
};
