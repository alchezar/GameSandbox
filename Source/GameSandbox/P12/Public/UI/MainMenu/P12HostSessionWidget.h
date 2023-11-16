// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12NetworkWidget.h"
#include "P12HostSessionWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP12HostSessionWidget : public UP12NetworkWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void ToggleNetworkType() override;
	void CreateSession();

private:
	void UpdateNetworkTypeText();
	UFUNCTION()
	void OnToggleLeftButtonClicked();
	UFUNCTION()
	void OnToggleRightButtonClicked();
	UFUNCTION()
	void OnUserServerNameCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnBackButtonClicked();
	UFUNCTION()
	void OnCreateButtonClicked();
	
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NetworkTypeText;
	UPROPERTY(meta = (BindWidget))
	UButton* ToggleLeftButton;	
	UPROPERTY(meta = (BindWidget))
	UButton* ToggleRightButton;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* UserServerNameTextBox;	
	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateButton;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName ServerName;
};
