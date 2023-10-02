// Copyright (C) 2023, IKinder

#include "P11/Public/Game/P11GameState.h"
#include "P11/Public/UI/Chat/P11Chat.h"

void AP11GameState::Multicast_MessageSendToGameState_Implementation(const FP11MessageInfo& MessageInfo) 
{
	MessageSendToGameState(MessageInfo);
}

void AP11GameState::MessageSendToGameState(const FP11MessageInfo& MessageInfo) const
{
	OnMessageSendToGameState.Broadcast(MessageInfo);
}
