// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ServerList.generated.h"

#define SERVER_LIST_LOADING_INDEX 0
#define SERVER_LIST_SUCCESS_INDEX 1
#define SERVER_LIST_FAILURE_INDEX 2

class UP11ServerInfo;
class UP11GameInstance;
class UWidgetSwitcher;
class UScrollBox;
class UButton;

DECLARE_MULTICAST_DELEGATE(FP11OnServerListBackButtonPressedSignature)

UCLASS()
class PROJECT11_API UP11ServerList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnBackHandle();
	UFUNCTION()
	void OnRefreshHandle();

	void OnFindSessionFailHandle();
	void OnFindSessionSuccessfulHandle(const FOnlineSessionSearchResult& SessionSearchResult);

public:
	FP11OnServerListBackButtonPressedSignature OnBackServerListBackPressed;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* RefreshBtn;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerListScrl;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ListSwitcher;

	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP11ServerInfo> ServerInfoClass;

private:
	UPROPERTY()
	UP11GameInstance* GameInstance;
};
