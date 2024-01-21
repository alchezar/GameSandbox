// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P13/Public/Library/P13Types.h"
#include "P13EnemySpawn.generated.h"

class AP13GameState;
class UBoxComponent;
class AP13CharacterEnemy;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             Enemy spawn point                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API AP13EnemySpawnPoint : public AActor
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13EnemySpawnPoint();

protected:
	virtual void BeginPlay() override;

	/* ------------------------------- This -------------------------------- */
public:
	bool TrySpawnEnemy();

protected:
	virtual void SpawnEnemy();
	void SpawnEnemyAt(const FTransform& SpawnTransform) const;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<AP13CharacterEnemy> EnemyClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	EP13LevelPhase Phase = EP13LevelPhase::Phase1;

private:
	TSoftObjectPtr<AP13GameState> CachedGameState;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Enemy spawn box                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
UCLASS()
class AP13EnemySpawnBox : public AP13EnemySpawnPoint
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13EnemySpawnBox();

	/* ------------------------------- This -------------------------------- */
public:
	virtual void SpawnEnemy() override;

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	UBoxComponent* SpawnBox;
};
