// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P11GameModeBase.generated.h"

struct FP11MessageInfo;
class AP11Character;

UCLASS()
class GAMESANDBOX_API AP11GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP11GameModeBase();
	void Respawn(AController* Controller);
	UFUNCTION(Server, Reliable)
	void Server_SendToGameMode(const FP11MessageInfo& MessageInfo);

protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<AP11Character> RespawnCharClass;
};
