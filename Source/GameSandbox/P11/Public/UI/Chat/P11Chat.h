// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P11Chat.generated.h"

class UP11ChatMessage;
class UScrollBox;

UCLASS()
class GAMESANDBOX_API UP11Chat : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnMessageSendHandle(const FString& Sender, const FString& Message);

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* MessagesScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP11ChatMessage> ChatMessageClass;
};
