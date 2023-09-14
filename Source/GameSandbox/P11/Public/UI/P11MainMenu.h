// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11MainMenu.generated.h"

class UEditableTextBox;
class AP11GameModeBase;
class UButton;

UCLASS()
class GAMESANDBOX_API UP11MainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateHandle();
	UFUNCTION()
	void OnSingleHandle();
	UFUNCTION()
	void OnJoinHandle();
	UFUNCTION()
	void OnQuitHandle();

protected:
#pragma region MiddleSection
	UPROPERTY(meta = (BindWidget))
	UButton* CreateBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* SingleBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* AddressBox;
#pragma endregion /* MiddleSection */
	
private:
	UPROPERTY()
	AP11GameModeBase* GameMode;
};
