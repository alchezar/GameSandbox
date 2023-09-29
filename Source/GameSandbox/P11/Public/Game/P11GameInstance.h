// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "P11GameInstance.generated.h"

class UP11SavePlayerInfo;

UCLASS()
class GAMESANDBOX_API UP11GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	FORCEINLINE UP11SavePlayerInfo* GetSavePlayerInfo() const { return SavePlayerInfo; }
	FORCEINLINE FString GetSlotName() const { return SlotName; }
	FORCEINLINE TSoftObjectPtr<UWorld> GetStartupMap() const { return StartupMap; }

	void Host(const int32 MaxPlayers, const bool bLAN, const FString& LevelURL, const FString& ServerName);
	void Join(uint32 Index);
	void RefreshServerList();

private:
	void FindSessionInterface();
	void OnCreateSessionCompleteHandle(FName SessionName, bool bSuccessful);
	void OnDestroySessionCompleteHandle(FName SessionName, bool bSuccessful);
	void OnFindSessionCompleteHandle(bool bSuccessful);
	void OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailureHandle(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Type, const FString& ErrorString);
	void CreateSession(const int32 MaxPlayers, const bool bLAN, const FString& LevelURL, const FString& ServerName);

private:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSoftObjectPtr<UWorld> StartupMap;
	UPROPERTY()
	UP11SavePlayerInfo* SavePlayerInfo;
	FString SlotName = "Info";

	IOnlineSessionPtr SessionInterface;
	FName CurrentSessionName;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName ServerNameKey = "ServerNameKey";
	FString TargetLevelURL;
};
