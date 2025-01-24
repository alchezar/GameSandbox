// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Intearface/P13NetworkInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Library/P13Types.h"
#include "P13GameInstance.generated.h"

#define SERVER_NAME_KEY "ServerNameKey"

UCLASS()
class PROJECT13_API UP13GameInstance : public UGameInstance, public IP13NetworkInterface
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13GameInstance() {}
	virtual void Init() override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual void HostSession(const int32 MaxPlayers, const bool bLan, const FString& CustomServerName) override;
	virtual void FindSessions(const bool bLan) override;
	virtual void JoinSession(const FOnlineSessionSearchResult& Result) override;
	virtual void DestroySession() override;

	/* ------------------------------- This -------------------------------- */
public:
	FP13WeaponInfo* GetWeaponInfoByID(const FName WeaponID) const;
	FP13WeaponDrop* GetWeaponDropByID(const FName WeaponID) const;
	const TMap<FString, FLinearColor>& GetPlayersColorsMap() const { return PlayersColorMap; }
	void SavePlayerColor(const FString& PlayerName, const FLinearColor PlayerColor);

private:
	void FindSessionInterface();
	void OnCreateSessionCompleteHandle(FName SessionName, bool bSuccess);
	void OnFindSessionCompleteHandle(bool bSuccess);
	void OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionCompleteHandle(FName SessionName, bool bSuccessful);

	/* ----------------------------- Variables ----------------------------- */
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	UDataTable* WeaponInfoTable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	UDataTable* WeaponDropTable = nullptr;

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	FName CurrentSessionName = "Game";
	FName StartLevelName = "MenuLevel_P13_";
	FName LobbyLevelName = "LobbyLevel_P13_";
	int32 MaxPlayersNum = 2;
	TMap<FString, FLinearColor> PlayersColorMap;
};
