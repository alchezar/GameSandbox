// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ARMainMenu.generated.h"

class UEditableTextBox;
class UARMenuButton;

UCLASS()
class GAMESANDBOX_API UARMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnHostButtonClicked();
	UFUNCTION()
	void OnJoinButtonClicked();
	UFUNCTION()
	void OnQuitButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	UARMenuButton* ButtonQuit;
	UPROPERTY(meta = (BindWidget))
	UARMenuButton* ButtonJoin;
	UPROPERTY(meta = (BindWidget))
	UARMenuButton* ButtonHost;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* IpFieldTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName LevelName = "ActionRoguelikeDemo";
};
