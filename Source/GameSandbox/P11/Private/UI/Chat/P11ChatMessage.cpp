// Copyright (C) 2023, IKinder

#include "P11/Public/UI/Chat/P11ChatMessage.h"

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
}

void UP11ChatMessage::AddMessage(const FString& Sender, const FString& Message)
{
	SenderString = Sender;
	MessageString = Message;
	SetTextBlocks();
}
