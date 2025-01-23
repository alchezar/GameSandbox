// Copyright (C) 2023, IKinder

#include "UI/Chat/P11ChatMessage.h"

#include "Components/TextBlock.h"

void UP11ChatMessage::NativeConstruct()
{
	Super::NativeConstruct();

	SetTextBlocks();
}

void UP11ChatMessage::SetTextBlocks()
{
	SenderText->SetText(FText::FromString(SenderString));
	MessageText->SetText(FText::FromString(MessageString));

	/* Set the message color depending on the message state. */
	FSlateColor MessageColor = FColor::White;
	if (MessageState == EP11MessageState::Login)
	{
		MessageColor = FColor::Green;
	}
	else if (MessageState == EP11MessageState::Logout)
	{
		MessageColor = FColor::Red;
	}
	MessageText->SetColorAndOpacity(MessageColor);
}

void UP11ChatMessage::AddMessage(const FString& Sender, const FString& Message)
{
	SenderString = Sender;
	MessageString = Message;
	SetTextBlocks();
}
