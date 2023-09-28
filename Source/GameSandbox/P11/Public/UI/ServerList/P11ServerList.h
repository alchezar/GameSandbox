// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ServerList.generated.h"

class UWidgetSwitcher;
class UScrollBox;
class UButton;

DECLARE_MULTICAST_DELEGATE(FP11OnServerListBackButtonPressedSignature)

UCLASS()
class GAMESANDBOX_API UP11ServerList : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnBackHandle();
	UFUNCTION()
	void OnRefreshHandle();

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
};
