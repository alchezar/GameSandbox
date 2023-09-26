// Copyright (C) 2023, IKinder

#include "P11/Public/Game/P11GameState.h"

void AP11GameState::Multicast_MessageSendToGameState_Implementation(const FString& Sender, const FString& Message)
{
	MessageSendToGameState(Sender, Message);
}

void AP11GameState::MessageSendToGameState(const FString& Sender, const FString& Message) const
{
	OnMessageSendToGameState.Broadcast(Sender, Message);
}
