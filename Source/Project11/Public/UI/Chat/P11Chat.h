// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P11ChatMessage.h"
#include "Blueprint/UserWidget.h"
#include "P11Chat.generated.h"

class UP11ChatMessage;
class UScrollBox;

USTRUCT(BlueprintType)
struct FP11MessageInfo
{
	GENERATED_BODY()

	FP11MessageInfo()
	{
		Sender = "Unknown";
		Message = "No message";
		MessageState = EP11MessageState::Player;
	}
	FP11MessageInfo(const FString& NewSender, const FString& NewMessage)
	{
		Sender = NewSender;
		Message = NewMessage;
		MessageState = EP11MessageState::Player;
	}
	FP11MessageInfo(const FString& NewSender, const FString& NewMessage, const EP11MessageState NewMessageState)
	{
		Sender = NewSender;
		Message = NewMessage;
		MessageState = NewMessageState;
	}

	UPROPERTY()
	FString Sender;
	UPROPERTY()
	FString Message;
	UPROPERTY()
	EP11MessageState MessageState;
};

UCLASS()
class PROJECT11_API UP11Chat : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetScrollBarVisibility(const bool bNewVisibility) { bScrollBarVisibility = bNewVisibility; }

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnMessageSendFromGameStateHandle(const FP11MessageInfo& MessageInfo);
	UFUNCTION()
	void OnMessageSendFromGameModeHandle(const FP11MessageInfo& MessageInfo);

private:
	void AddMessageToChat(const FP11MessageInfo& MessageInfo);
	void DelayedChatFading(const float Delay = 3.f, const float Duration = 1.f);
	void SmoothChatFading(const float Duration);

protected:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* MessagesScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP11ChatMessage> ChatMessageClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bScrollBarVisibility = true;

private:
	FTimerHandle FadeTimer;
};
