// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "P11GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FP11OnMessageSendToGameStateSignature, const FString&, Sender, const FString&, Message);

UCLASS()
class GAMESANDBOX_API AP11GameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_MessageSendToGameState(const FString& Sender, const FString& Message);
	UFUNCTION()
	void MessageSendToGameState(const FString& Sender, const FString& Message) const;

public:
	FP11OnMessageSendToGameStateSignature OnMessageSendToGameState;
};
