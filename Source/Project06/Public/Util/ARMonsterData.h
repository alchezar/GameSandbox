// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ARMonsterData.generated.h"

class UARAbility;

UCLASS()
class PROJECT06_API UARMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Spawn")
	TSubclassOf<AActor> MonsterClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Spawn")
	TArray<TSubclassOf<UARAbility>> Abilities;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Spawn")
	UTexture2D* Icon;
	
};
