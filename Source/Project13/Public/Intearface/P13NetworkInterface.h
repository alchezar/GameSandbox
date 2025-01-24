// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P13NetworkInterface.generated.h"

UINTERFACE()
class UP13NetworkInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT13_API IP13NetworkInterface
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnFindSessionsCompleteHandle, TArray<FOnlineSessionSearchResult> /*SearchResults*/)
	FP13OnFindSessionsCompleteHandle OnFindSessionsComplete;

public:
	virtual void HostSession(const int32 MaxPlayers, const bool bLan, const FString& CustomServerName) = 0;
	virtual void FindSessions(const bool bLan) = 0;
	virtual void JoinSession(const FOnlineSessionSearchResult& Result) = 0;
	virtual void DestroySession() = 0;
};
