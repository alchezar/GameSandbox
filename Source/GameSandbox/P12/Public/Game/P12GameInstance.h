// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "P12GameInstance.generated.h"

class FOnlineSessionSearch;
class FOnlineSessionSettings;

DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnMatchFound, bool)

UCLASS()
class GAMESANDBOX_API UP12GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UP12GameInstance();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/* Allow to Exec console commands from GameInstance. */
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

private:
	void DisplayNetworkErrorMessage(const FString& ErrorMessage);
	static IOnlineSessionPtr GetSessionFromOnlineSubsystem();

public:
	FP12OnMatchFound OnMatchFound;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	FName LobbyMapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	FName MainMenuMapName;

	UPROPERTY(Replicated, EditAnywhere, Category = "C++ | Server settings")
	FName ServerName;
	UPROPERTY(Replicated, EditAnywhere, Category = "C++ | Server settings")
	uint32 MaxPlayers;

#pragma region CreatingNetworkSession
protected:
	/**
	 * Function to host game.
	 * @param UserId		User that started the request.
	 * @param SessionName	Name of the session.
	 * @param bLAN			Is this LAN game?
	 * @param bPresence		Is the session to create a presence session?
	 * @param MaxNumPlayers Number of maximum allowed players on this session (server). 
	 */
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bLAN, bool bPresence, int32 MaxNumPlayers);
	/**
	 * Function fired when a session create request has completed.
	 * @param SessionName the name of the session this callback is for.
	 * @param bSuccessful true if the async action completed without error, false if there was an error.
	 */
	void OnCreateSessionComplete(FName SessionName, bool bSuccessful);
	/**
	 * Function fired when a session start request has completed.
	 * @param SessionName the name of the session this callback is for.
	 * @param bSuccessful true if the async action completed without error, false if there was an error.
	 */		
	void OnStartOnlineGameComplete(FName SessionName, bool bSuccessful);
	
protected:
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	/* Delegate called when session is created. */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session is started. */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	/* Handles to register delegates for creating/starting sessions. */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	
#pragma endregion CreatingNetworkSession

#pragma region FindingNetworkSession
protected:
	/**
	 * Find an online session.
	 * @param UserId	User that initiated the request.
	 * @param bLAN		Are we searching LAN matches?
	 * @param bPresence	Are we searching presence session?
	 */
	void FindSession(TSharedPtr<const FUniqueNetId> UserId, bool bLAN, bool bPresence);
	/**
	 * Delegate fired when a session search query has completed.
	 * @param bSuccessful true if the async action completed without error, false if there was an error.
	 */
	void OnFindSessionComplete(const bool bSuccessful);
	
protected:
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	/* Delegate for searching for the sessions. */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	/* Handle for register delegates for searching a session. */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	
#pragma endregion FindingNetworkSession   
};
