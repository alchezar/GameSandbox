// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "P13MenuWidget.h"
#include "P13LobbyWidget.generated.h"

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

	UPROPERTY(EditAnywhere, Category = "C++")
	FString CustomServerName;
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
protected:
	UFUNCTION()
	void OnReadyButtonClickedHandle();
	UFUNCTION()
	void OnExitButtonClickedHandle();

private:
	void InitWidget();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerRoleText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* StartReadyText;
	UPROPERTY(meta = (BindWidget))
	UButton* ReadyButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Color Button                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UCLASS()
class UP13LevelButtonWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnColorButtonClickedHandle();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ColorButton;
	
};
