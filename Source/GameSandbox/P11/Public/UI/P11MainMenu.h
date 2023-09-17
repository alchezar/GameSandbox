// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11/Public/Game/P11SavePlayerInfo.h"
#include "P11MainMenu.generated.h"

class UCheckBox;
class UP11MenuPopUp;
class UP11MenuOption;
class AP11PlayerController;
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

private:
	UFUNCTION()
	void OnVisibilityChangedHandle(ESlateVisibility InVisibility);
	
	void MiddleSection();
	UFUNCTION()
	void OnCreateHandle();
	UFUNCTION()
	void OnSingleHandle();
	UFUNCTION()
	void OnJoinHandle();
	UFUNCTION()
	void OnQuitHandle();
	
	void RightSection();
	UFUNCTION()
	void OnJediHandle();
	UFUNCTION()
	void OnSythHandle();
	UFUNCTION()
	void OnApplyHandle();

	void LeftSection();
	UFUNCTION()
	void OnShadowOptionHandle(float Value);
	UFUNCTION()
	void OnReflectOptionHandle(float Value);
	UFUNCTION()
	void OnGiOptionHandle(float Value);
	UFUNCTION()
	void OnVfxOptionHandle(float Value);
	UFUNCTION()
	void OnTextureOptionHandle(float Value);
	UFUNCTION()
	void OnShadingOptionHandle(float Value);
	UFUNCTION()
	void OnFoliageOptionHandle(float Value);
	UFUNCTION()
	void OnTsrOptionHandle(float Value);
	UFUNCTION()
	void OnResolutionOptionHandle(FString SelectedItem);
	UFUNCTION()
	void OnFpsCheckBoxHandle(bool bIsChecked);
	UFUNCTION()
	void OnFullscreenHandle();
	UFUNCTION()
	void OnWindowedFullHandle();
	UFUNCTION()
	void OnWindowedHandle();
	UFUNCTION()
	void OnDefaultHandle();
	UFUNCTION()
	void OnSaveHandle();

	void ShowCurrentSettings() const;
	void SetActiveScreenModeButtons(const EWindowMode::Type CurrentMode) const;
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

#pragma region LeftSection
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* ShadowOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* ReflectOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* GiOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* VfxOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* TextureOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* ShadingOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* FoliageOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuOption* TsrOption;
	UPROPERTY(meta = (BindWidget))
	UP11MenuPopUp* ResolutionOption;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* FpsCheckBox;
	UPROPERTY(meta = (BindWidget))
	UButton* FullscreenBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* WindowedFullBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* WindowedBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* DefaultBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* SaveBtn;
#pragma endregion /* LeftSection */
	
private:
	EP11PlayerSide PlayerSide = EP11PlayerSide::Jedi;
	UPROPERTY()
	UP11GameInstance* GameInstance;
	UPROPERTY()
	AP11PlayerController* Controller;
	UPROPERTY()
	AP11CharUI* CharUI;
	UPROPERTY()
	UGameUserSettings* UserSettings;
	UPROPERTY()
	UP11SavePlayerInfo* SavedInfo;
};
