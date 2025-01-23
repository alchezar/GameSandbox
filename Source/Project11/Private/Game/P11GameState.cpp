// Copyright (C) 2023, IKinder

#include "Game/P11GameState.h"
#include "UI/Chat/P11Chat.h"

void AP11GameState::Multicast_MessageSendToGameState_Implementation(const FP11MessageInfo& MessageInfo) 
{
	MessageSendToGameState(MessageInfo);
}

void AP11GameState::MessageSendToGameState(const FP11MessageInfo& MessageInfo) const
{
	OnMessageSendToGameState.Broadcast(MessageInfo);
}
