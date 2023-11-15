// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
// #include "Interfaces/OnlineSessionDelegates.h"
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
	virtual void Init() override;
	virtual void Shutdown() override;
	/* Allow to Exec console commands from GameInstance. */
	virtual bool ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor) override;

private:
	void DisplayNetworkErrorMessage(const FString& ErrorMessage);
	static IOnlineSessionPtr GetSessionsFromOnlineSubsystem();

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

	/*------ CreatingNetworkSession ------*/
protected:
	/**
	 * Function to host game.
	 * @param UserId		User that started the request.
	 * @param SessionName	Name of the session.
	 * @param bLAN			Is this LAN game?
	 * @param bPresence		Is the session to create a presence session?
	 * @param MaxNumPlayers Number of maximum allowed players on this session (server).
	 * @return				true if success, false otherwise.
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
	
	/*------ FindingNetworkSession ------*/
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
	
	/*------ JoiningNetworkSession ------*/
protected:
	/**
	 * Join a session via the search result.
	 * @param UserId		User that joining.
	 * @param SessionName	of the session.
	 * @param SearchResult	session to join.
	 * @return				true if success, false otherwise.
	 */
	bool JoinFoundOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);
	/**
	 * Delegate fired when a session join request has completed
	 * @param SessionName the name of the session this callback is for
	 * @param Result Success if the async action completed without error, or other enum value if there was an error
	 */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:
	/* Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	/* Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/*------ DestroyingASession ------*/
protected:
	/**
	* Delegate fired when a destroying an online session has completed.
	* @param SessionName the name of the session this callback is for.
	* @param bSuccessful true if the async action completed without error, false if there was an error.
	*/
	void OnDestroySessionComplete(FName SessionName, bool bSuccessful);

protected:
	/* Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	/* Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/*------ OnFailure ------*/
protected:
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage);
	void OnTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorMessage);

protected:
	FDelegateHandle OnNetworkFailureEventHandle;
	FDelegateHandle OnTravelFailureEventHandle;
};
