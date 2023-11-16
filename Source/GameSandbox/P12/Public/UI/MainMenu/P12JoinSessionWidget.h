// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12NetworkWidget.h"
#include "P12JoinSessionWidget.generated.h"

class UWidgetSwitcher;
class UTextBlock;
class UButton;
class UP12GameInstance;

UENUM(BlueprintType)
enum class EP12SearchingSessionState : uint8
{
	None = 0,
	Searching,
	Found
};

UCLASS()
class GAMESANDBOX_API UP12JoinSessionWidget : public UP12NetworkWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void CloseWidget() override;

	void FindOnlineSession();
	void JoinOnlineSession();
	void OnMatchFound(bool bSuccessful);

private:
	void UpdateNetworkTypeText();
	UFUNCTION()
	void OnToggleSideButtonClicked();
	UFUNCTION()
	void OnBackButtonClicked();
	UFUNCTION()
	void OnAcceptButtonClicked();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NetworkTypeText;
	UPROPERTY(meta = (BindWidget))
	UButton* ToggleLeftButton;	
	UPROPERTY(meta = (BindWidget))
	UButton* ToggleRightButton;
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* AcceptButton;	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ToggleWidgets;
	
	UPROPERTY(EditAnywhere, Transient, BlueprintReadOnly, Category = "C++")
	EP12SearchingSessionState SearchingSessionState;

private:
	TWeakObjectPtr<UP12GameInstance> CachedGameInstance;
	
};
