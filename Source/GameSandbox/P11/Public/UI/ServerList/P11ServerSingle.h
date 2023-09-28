// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ServerSingle.generated.h"

class UCheckBox;
class UComboBoxString;
class UEditableTextBox;
class UButton;

DECLARE_MULTICAST_DELEGATE(FP11OnServerSingleBackButtonPressedSignature)

UCLASS()
class GAMESANDBOX_API UP11ServerSingle : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	virtual void OnBackHandle();
	UFUNCTION()
	virtual void OnCreateHandle();

public:
	FP11OnServerSingleBackButtonPressedSignature OnSingleBackBackButtonPressed;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	// UPROPERTY(meta = (BindWidget))
	// UEditableTextBox* ServerNameTextBox;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ServerMapComboBox;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MaxPlayersTextBox;
	// UPROPERTY(meta = (BindWidget))
	// UCheckBox* LanCheckBox;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateBtn;
};
