// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interface/P8MenuInterface.h"
#include "P8GameInstance.generated.h"

class UP8MainMenuWidget;
class FOnlineSessionSearch;

UCLASS()
class PROJECT08_API UP8GameInstance : public UGameInstance, public IP8MenuInterface
{
	GENERATED_BODY()

public:
	explicit UP8GameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;              // UGameInstance
	virtual void Host() override;              // IP8MenuInterface
	virtual void Join(uint32 Index) override;  // IP8MenuInterface
	virtual void RefreshServerList() override; // IP8MenuInterface
	FORCEINLINE FString GetStartURL() const { return StartLevelURL; };
	FORCEINLINE FString GetLobbyURL() const { return LobbyLevelURL; };
	FORCEINLINE FString GetGameURL() const { return GameLevelURL; };
	void StartSession();

protected:
	UFUNCTION(Exec)
	void LoadMenu() const;

private:
	void OnCreateSessionCompleteHandle(FName SessionName, bool bWasSuccessful);
	void OnDeleteSessionCompleteHandle(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleteHandle(bool bWasSuccessful);
	void OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailureHandle(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Type, const FString& ErrorString);
	void CreateSession();
	void FindMenuWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString StartLevelURL = "/Game/Project/P8/Level/Start";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString LobbyLevelURL = "/Game/Project/P8/Level/Lobby";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString GameLevelURL = "/Game/Project/P8/Level/MasterMultiplayer";

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> ServerRowClass;
	IOnlineSessionPtr SessionInterface;
	FName CurrentSessionName = FName("Game");
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	UPROPERTY()
	UP8MainMenuWidget* MainMenuWidget;
	FName CustomServerNameKey = FName("CustomServerNameKey");
};
