// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ServerSingle.generated.h"

class UP11GameInstance;
class UCheckBox;
class UComboBoxString;
class UEditableTextBox;
class UButton;

DECLARE_MULTICAST_DELEGATE(FP11OnServerSingleBackButtonPressedSignature)

UCLASS()
class PROJECT11_API UP11ServerSingle : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	virtual void OnBackHandle();
	UFUNCTION()
	virtual void OnCreateHandle();

	int32 GetMaxPlayersNum() const;

private:
	void FindAllLevels();

public:
	FP11OnServerSingleBackButtonPressedSignature OnSingleBackBackButtonPressed;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* BackBtn;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ServerMapComboBox;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MaxPlayersTextBox;
	UPROPERTY(meta = (BindWidget))
	UButton* CreateBtn;

	UPROPERTY(EditAnywhere, Category = "C++")
	FName SearchPath = "/Game/Project/PP11/Level";

	TArray<FString> LevelAddresses;
	UPROPERTY()
	UP11GameInstance* GameInstance;
};
