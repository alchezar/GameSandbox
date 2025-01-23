// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11ChatBox.generated.h"

class UButton;
class AP11PlayerController;
class UEditableTextBox;
class UWidgetSwitcher;

UCLASS()
class PROJECT11_API UP11ChatBox : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE UEditableTextBox* GetMessageBox() const { return MessageBox; }
	int32 GetSwitcherIndex() const;
	void ExtendChat(bool bExtended);
	
protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnMessageCommittedHandle(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION()
	void OnSendPressedHandle();

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ChatSwitcher;
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* MessageBox;
	UPROPERTY(meta = (BindWidget))
	UButton* SendBtn;

private:
	FString Sender = "";
	UPROPERTY()
	AP11PlayerController* Controller;
};
