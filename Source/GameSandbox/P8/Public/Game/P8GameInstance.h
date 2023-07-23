// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P8/Public/Interface/P8MenuInterface.h"
#include "P8GameInstance.generated.h"

UCLASS()
class GAMESANDBOX_API UP8GameInstance : public UGameInstance, public IP8MenuInterface
{
	GENERATED_BODY()

public:
	UP8GameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;                       // UGameInstance
	virtual void Host() override;                       // IP8MenuInterface
	virtual void Join(const FString& Address) override; // IP8MenuInterface
	FORCEINLINE FString GetLobbyURL() const { return LobbyLevelURL; };

protected:
	UFUNCTION(Exec)
	void LoadMenu() const;
	UFUNCTION(Exec)
	void HostServer() const;
	UFUNCTION(Exec)
	void JoinServer(const FString& Address) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString LobbyLevelURL = "/Game/Project/P8/Level/Lobby";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | URL")
	FString GameLevelURL = "/Game/Project/P8/Level/MasterMultiplayer";

private:
	TSubclassOf<UUserWidget> MainMenuClass;
};
