// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/P12FirstPersonCharacter.h"
#include "P12MainMenuWidget.generated.h"

class UP12JoinSessionWidget;
class UP12HostSessionWidget;
class UWidgetSwitcher;
class UButton;

UCLASS()
class PROJECT12_API UP12MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void CacheHUD(const AP12HUD* HUD);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnReturnButtonClickedHandle();
	UFUNCTION()
	void OnHostButtonClickedHandle();
	UFUNCTION()
	void OnJoinButtonClickedHandle();
	void OnNetworkWidgetClosedHandle();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnButton;
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	UP12HostSessionWidget* HostSessionWidget;
	UPROPERTY(meta = (BindWidget))
	UP12JoinSessionWidget* JoinSessionWidget;

private:
	TSoftObjectPtr<AP12HUD> CachedHUD;
	int32 MainMenuIndex = 0;
	int32 CreateSessionIndex = 1;
	int32 JoinSessionIndex = 2;
};
