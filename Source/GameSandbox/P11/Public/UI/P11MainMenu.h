// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11/Public/Game/P11SavePlayerInfo.h"
#include "P11MainMenu.generated.h"

class AP11CharUI;
class UP11GameInstance;
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
	void OnVisibilityChangedHandle(ESlateVisibility InVisibility);
	
	UFUNCTION()
	void OnCreateHandle();
	UFUNCTION()
	void OnSingleHandle();
	UFUNCTION()
	void OnJoinHandle();
	UFUNCTION()
	void OnQuitHandle();
	
	UFUNCTION()
	void OnJediHandle();
	UFUNCTION()
	void OnSythHandle();
	UFUNCTION()
	void OnApplyHandle();

private:
	static void ActivateButton(UButton* Button, const bool bActive);

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

#pragma region RightSection
	UPROPERTY(meta = (BindWidget))
	UButton* JediBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* SythBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* ApplyBtn;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* NameBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<AP11CharUI> CharUIClass;
#pragma endregion /* RightSection */
	
private:
	EP11PlayerSide PlayerSide = EP11PlayerSide::Jedi;
	UPROPERTY()
	UP11GameInstance* GameInstance;
	UPROPERTY()
	AP11GameModeBase* GameMode;
	UPROPERTY()
	AP11CharUI* CharUI;
};
