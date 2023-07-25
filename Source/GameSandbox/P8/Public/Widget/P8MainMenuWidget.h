// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P8/Public/Interface/P8MenuInterface.h"
#include "P8MainMenuWidget.generated.h"

class UP8ServerRow;
class UScrollBox;
class UWidgetSwitcher;
class UEditableTextBox;
class UP8GameInstance;
class UButton;

UCLASS()
class GAMESANDBOX_API UP8MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void Setup();
	void SetServerList(TArray<FString> ServerNames);
	void SetSelectedIndex(const uint32 Index);
	
protected:
	virtual void NativeConstruct() override;
	/* Widget index 0 */
	UFUNCTION()
	void OnHostClickedHandle();
	UFUNCTION()
	void OnJoinClickedHandle();
	UFUNCTION()
	void OnQuitClickedHandle();
	/* Widget index 1 */
	UFUNCTION()
	void OnCancelClickedHandle();
	UFUNCTION()
	void OnConnectClickedHandle();
	
protected:
	/* Switch between widget pages */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* JoinWidgetSwitcher;
	/* Widget index 0 */
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
	/* Widget index 1 */
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ConnectButton;
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerListScrollBox;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Widget")
	TSubclassOf<UUserWidget> ServerRowClass;

private:
	IP8MenuInterface* MenuInterface;
	UPROPERTY()
	UP8GameInstance* GameInstance;
	TOptional<uint32> SelectedIndex;
};
