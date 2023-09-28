// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P11ServerSingle.h"
#include "P11ServerCreate.generated.h"

class UCheckBox;
// class UComboBoxString;
class UEditableTextBox;
// class UButton;

DECLARE_MULTICAST_DELEGATE(FP11OnServerCreateBackButtonPressedSignature)

UCLASS()
class GAMESANDBOX_API UP11ServerCreate : public UP11ServerSingle
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	// UFUNCTION()
	virtual void OnBackHandle() override;
	// UFUNCTION()
	virtual void OnCreateHandle() override;

public:
	FP11OnServerCreateBackButtonPressedSignature OnCreateBackButtonPressed;

protected:
	// UPROPERTY(meta = (BindWidget))
	// UButton* BackBtn;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* ServerNameTextBox;
	// UPROPERTY(meta = (BindWidget))
	// UComboBoxString* ServerMapComboBox;
	// UPROPERTY(meta = (BindWidget))
	// UEditableTextBox* MaxPlayersTextBox;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* LanCheckBox;
	// UPROPERTY(meta = (BindWidget))
	// UButton* CreateBtn;
};
