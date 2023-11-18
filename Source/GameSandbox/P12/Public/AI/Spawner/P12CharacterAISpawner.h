// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12CharacterAISpawner.generated.h"

class AP12AICharacter;

UCLASS()
class GAMESANDBOX_API AP12CharacterAISpawner : public AActor
{
	GENERATED_BODY()

public:
	AP12CharacterAISpawner();
	void SpawnAI();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TSubclassOf<AP12AICharacter> CharacterClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	bool bSpawnOnStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	bool bDoOnce = false;

private:
	bool bCanSpawn = true;
};
