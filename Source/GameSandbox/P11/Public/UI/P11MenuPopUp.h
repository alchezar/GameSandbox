// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11MenuPopUp.generated.h"

class UComboBoxString;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP11OnComboBoxSelectionChangedSignature, FString, SelectedItem);

UCLASS()
class GAMESANDBOX_API UP11MenuPopUp : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UComboBoxString* GetComboBox() const { return ComboBox; }
	
protected:
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void OnComboBoxSelectionChangedHandle(FString SelectedItem, ESelectInfo::Type SelectionType);

public:
	FP11OnComboBoxSelectionChangedSignature OnComboBoxSelectionChanged;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FText CustomName = FText::FromString("setting");
};
