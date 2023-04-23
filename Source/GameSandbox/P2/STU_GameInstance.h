// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_CoreTypes.h"
#include "Engine/GameInstance.h"
#include "STU_GameInstance.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FLevelData GetStartupLevel() const;
	FName GetMainLevelName() const;
	TArray<FLevelData> GetLevelsData() const;

	void SetStartupLevel(const FLevelData& Data);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Game")
	TArray<FLevelData> LevelsData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Game")
	FName MainLevelName = NAME_None;

private:
	FLevelData StartupLevel;
};
