// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "P12DebugSubsystem.generated.h"

UCLASS()
class PROJECT12_API UP12DebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	bool GetIsDebugCategoryEnabled(const FName& CategoryName) const;

private:
	UFUNCTION(Exec)
	void EnableDebugCategory(const FName& CategoryName, bool bEnabled);

private:
	TMap<FName, bool> DebugCategories;
};
