// Copyright (C) 2023, IKinder

#include "P11/Public/UI/Chat/P11Chat.h"

#include "Components/ScrollBox.h"
#include "P11/Public/Game/P11GameState.h"
#include "P11/Public/UI/Chat/P11ChatMessage.h"

void UP11Chat::NativeConstruct()
{
	Super::NativeConstruct();

	AP11GameState* GameState = Cast<AP11GameState>(GetWorld()->GetGameState());
	if (!GameState)
	{
		return;
	}
	GameState->OnMessageSendToGameState.AddDynamic(this, &ThisClass::OnMessageSendHandle);
}

void UP11Chat::OnMessageSendHandle(const FString& Sender, const FString& Message)
{
	if (!ChatMessageClass)
	{
		return;
	}
	if (UP11ChatMessage* ChatMessage = CreateWidget<UP11ChatMessage>(GetOwningPlayer(), ChatMessageClass))
	{
		ChatMessage->AddMessage(Sender, Message);
		// ChatMessage->AddToViewport();
		MessagesScrollBox->AddChild(ChatMessage);
	}
}
