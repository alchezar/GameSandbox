// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P11/Public/UI/Chat/P11Chat.h"
#include "P11GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP11OnMessageSendToGameStateSignature, const FP11MessageInfo&, MessageInfo);

UCLASS()
class GAMESANDBOX_API AP11GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_MessageSendToGameState(const FP11MessageInfo& MessageInfo);
	UFUNCTION()
	void MessageSendToGameState(const FP11MessageInfo& MessageInfo) const;

public:
	FP11OnMessageSendToGameStateSignature OnMessageSendToGameState;
};
