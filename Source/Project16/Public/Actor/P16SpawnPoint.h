// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "P16SpawnPoint.generated.h"

class AP16Enemy;

USTRUCT(BlueprintType)
struct FP16EnemyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AP16Enemy> Class = nullptr;
	UPROPERTY(EditAnywhere)
	int32 Level = 1;
};

UCLASS()
class PROJECT16_API AP16SpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16SpawnPoint();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	virtual void Spawn();

protected:
	FP16EnemyInfo GetRandomSpawnInfo();

	void SpawnOneEnemy(const FP16EnemyInfo& EnemyInfo, const FTransform& Transform) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TArray<FP16EnemyInfo> EnemySpawnInfos = {};
};
