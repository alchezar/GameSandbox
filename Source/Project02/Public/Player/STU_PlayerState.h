// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STU_PlayerState.generated.h"

UCLASS()
class PROJECT02_API ASTU_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASTU_PlayerState();
	virtual void Tick(float DeltaTime) override;

	int32        GetTeamID() const;
	FLinearColor GetTeamColor() const;
	int32        GetKillsNum() const;
	int32        GetDeathNum() const;
	FLinearColor GetBlasterColor() const;

	void SetTeamID(int32 NewID);
	void SetTeamColor(const FLinearColor& NewTeamColor);
	void SetBlasterColor(const FLinearColor& NewBlasterColor);

	void AddKill();
	void AddDeath();
	void LogInfo();

protected:
	virtual void BeginPlay() override;

private:
	int32        TeamID = 1;
	FLinearColor TeamColor;
	FLinearColor BlasterColor;
	int32        KillsNum  = 0;
	int32        DeathsNum = 0;
};
