// Copyright (C) 2023, IKinder

#include "UI/Chat/P11Chat.h"

#include "Components/ScrollBox.h"
#include "Game/P11GameState.h"
#include "Player/P11PlayerController.h"
#include "UI/Chat/P11ChatMessage.h"

void UP11Chat::NativeConstruct()
{
	Super::NativeConstruct();

	MessagesScrollBox->SetScrollBarVisibility(bScrollBarVisibility ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	AP11GameState* GameState = Cast<AP11GameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return;
	}
	GameState->OnMessageSendToGameState.AddDynamic(this, &ThisClass::OnMessageSendFromGameStateHandle);

	AP11PlayerController* PlayerController = GetOwningPlayer<AP11PlayerController>();
	if (!PlayerController)
	{
		return;
	}
	PlayerController->OnMessageSendToGameMode.AddDynamic(this, &ThisClass::OnMessageSendFromGameModeHandle);
}

void UP11Chat::OnMessageSendFromGameStateHandle(const FP11MessageInfo& MessageInfo)
{
	AddMessageToChat(MessageInfo);
}

void UP11Chat::OnMessageSendFromGameModeHandle(const FP11MessageInfo& MessageInfo)
{
	AddMessageToChat(MessageInfo);
}

void UP11Chat::AddMessageToChat(const FP11MessageInfo& MessageInfo)
{
	if (!ChatMessageClass)
	{
		return;
	}
	UP11ChatMessage* ChatMessage = CreateWidget<UP11ChatMessage>(GetOwningPlayer(), ChatMessageClass);
	if (!ChatMessage)
	{
		return;
	}
	ChatMessage->AddMessage(MessageInfo.Sender, MessageInfo.Message);
	ChatMessage->MessageState = MessageInfo.MessageState;
	MessagesScrollBox->AddChild(ChatMessage);
	
	/* Always show last messages. */
	MessagesScrollBox->ScrollToEnd();
	/* Show only last 30 messages. */
	if (MessagesScrollBox->GetChildrenCount() > 30)
	{
		MessagesScrollBox->RemoveChildAt(0);
	}
	/* Fade out after delay. */
	DelayedChatFading();
}

void UP11Chat::DelayedChatFading(const float Delay, const float Duration)
{
	GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
	/* Don`t fade out if this chat is extended. */   
	if (bScrollBarVisibility) 
	{
		return;
	}
	MessagesScrollBox->SetRenderOpacity(1.f);
	
	FTimerDelegate FadeDelegate;
	FadeDelegate.BindUObject(this, &ThisClass::SmoothChatFading, Duration);
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, FadeDelegate, GetWorld()->GetDeltaSeconds(), true, Delay);
}

void UP11Chat::SmoothChatFading(const float Duration)
{
	float NextTickOpacity = MessagesScrollBox->GetRenderOpacity() - GetWorld()->GetDeltaSeconds() * Duration;
	if (NextTickOpacity <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimer);
		NextTickOpacity = 0.f;
	}
	MessagesScrollBox->SetRenderOpacity(NextTickOpacity);
}
