// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

private:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSoftObjectPtr<UWorld> StartupMap;
	UPROPERTY()
	UP11SavePlayerInfo* SavePlayerInfo;
	FString SlotName = "Info";
};
