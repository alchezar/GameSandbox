// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "P13MenuWidget.h"
#include "P13/Public/Game/P13LobbyGameMode.h"
#include "P13LobbyWidget.generated.h"

class UUniformGridPanel;
class UP13ColorButtonWidget;
class UEditableText;
class UP13LobbyLevelSelectWidget;
class AP13LobbyPlayerController;
class UP13SessionSelectWidget;
class IP13NetworkInterface;
class UCircularThrobber;
class UCheckBox;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Start Menu                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13StartMenuWidget : public UP13MenuWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void OnPlayButtonPressed() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnHostButtonClickedHandle();
	UFUNCTION()
	void OnFindButtonClickedHandle();
	UFUNCTION()
	void OnLanStateChangedHandle(bool bChecked);
	UFUNCTION()
	void OnSessionNameCommittedHandle(const FText& Text, ETextCommit::Type CommitMethod);
	void OnFindSessionsCompleteHandle(TArray<FOnlineSessionSearchResult> OnlineSessionSearchResults);
	void OnSessionSelectedHandle(const FOnlineSessionSearchResult& SelectedResult);

private:
	void ClearSessionsList();
	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* FindButton;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* IsLanCheckBox;
	UPROPERTY(meta = (BindWidget))
	UCircularThrobber* SearchingThrobber;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* SessionsVerticalBox;
	UPROPERTY(meta = (BindWidget))
	UEditableText* SessionNameText;

	UPROPERTY(EditAnywhere, Category = "C++")
	FString CustomServerName = "ServerName";
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13SessionSelectWidget> SessionButtonWidgetClass;

private:
	IP13NetworkInterface* GameInstanceNetwork;
	FOnlineSessionSearchResult SelectedSessionCached;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Session Select                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13SessionSelectWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	void InitSessionButton(const FOnlineSessionSearchResult& SessionInfo);

protected:
	UFUNCTION()
	void OnSessionButtonPressedHandle();

	/* ----------------------------- Delegates ----------------------------- */
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnSessionSelectedHandle, const FOnlineSessionSearchResult& /*SelectedResult*/)
	FP13OnSessionSelectedHandle OnSessionSelected;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* SelectButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ServerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxPlayersText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentPlayersText;

private:
	FOnlineSessionSearchResult SelectedResult;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Lobby Menu                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UCLASS()
class UP13LobbyMenuWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE FLinearColor GetCurrentOccupiedColor() const { return CurrentOccupiedColor; }
	void UpdateLevelName(const FText& SelectedLevelName) const;
	void SelectLevelName(const FText& SelectedLevelName, FName SelectedLevelAddress);
	void UpdateReselectedColor(const FLinearColor ReleasedColor, const FLinearColor OccupiedColor);

protected:
	void SwitchColorOccupation(FLinearColor Color, const bool bOccupy);

	UFUNCTION()
	void OnReadyButtonClickedHandle();
	UFUNCTION()
	void OnExitButtonClickedHandle();

private:
	void InitWidget();
	void FindLevels();
	void CreateColorPalette();
	void ClearLevels() const;
	void ClearColors() const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerRoleText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StartReadyText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SelectedMapText;

	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelsHorizontalBox;
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ColorsGrid;

	UPROPERTY(EditAnywhere, Category = "C++")
	UDataTable* LevelSelectTable;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13LobbyLevelSelectWidget> LevelButtonWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13ColorButtonWidget> ColorButtonWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TArray<FLinearColor> ColorVariations;

private:
	bool bServer = false;
	TSoftObjectPtr<AP13LobbyPlayerController> CachedLobbyController;
	FName GameLevelAddress;
	FLinearColor CurrentOccupiedColor = FLinearColor::White;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Level Select                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13LobbyLevelSelectWidget : public UP13LevelSelectWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void OnLevelButtonPressed() override;

	/* ------------------------------- This -------------------------------- */
public:
	void InitLobbyLevelButton(const FP13LevelSelect* NewLevel, const UP13LobbyMenuWidget* LobbyMenu);

	/* ----------------------------- Variables ----------------------------- */
private:
	TSoftObjectPtr<UP13LobbyMenuWidget> CachedLobbyMenu;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Color Button                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UCLASS()
class UP13ColorButtonWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE FLinearColor GetButtonColor() const { return ButtonColor; }
	void InitColorButton(FLinearColor LinearColor);
	void SetIsColorEnabled(const bool bEnable) const;

protected:
	UFUNCTION()
	void OnColorButtonClickedHandle();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ColorButton;
	UPROPERTY(meta = (BindWidget))
	UImage* ColorImage;
	UPROPERTY(meta = (BindWidget))
	UImage* OccupiedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FLinearColor ButtonColor;

private:
	bool bOccupied = false;
};
