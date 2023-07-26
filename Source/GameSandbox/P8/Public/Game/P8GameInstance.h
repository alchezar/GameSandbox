// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "P8/Public/Interface/P8MenuInterface.h"
#include "P8GameInstance.generated.h"

class UP8MainMenuWidget;
class FOnlineSessionSearch;

UCLASS()
class GAMESANDBOX_API UP8GameInstance : public UGameInstance, public IP8MenuInterface
{
	GENERATED_BODY()

public:
	UP8GameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;              // UGameInstance
	virtual void Host() override;              // IP8MenuInterface
	virtual void Join(uint32 Index) override;  // IP8MenuInterface
	virtual void RefreshServerList() override; // IP8MenuInterface
	FORCEINLINE FString GetLobbyURL() const { return LobbyLevelURL; };

protected:
	UFUNCTION(Exec)
	void LoadMenu() const;

private:
	void OnCreateSessionCompleteHandle(FName SessionName, bool bWasSuccessful);
	void OnDeleteSessionCompleteHandle(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleteHandle(bool bWasSuccessful);
	void OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession();
	void FindMenuWidget();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString LobbyLevelURL = "/Game/Project/P8/Level/Lobby";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString GameLevelURL = "/Game/Project/P8/Level/MasterMultiplayer";

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> ServerRowClass;
	IOnlineSessionPtr SessionInterface;
	FName CurrentSessionName = FName("MySessionGame");
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	UPROPERTY()
	UP8MainMenuWidget* MainMenuWidget;
};
