// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12CharacterAISpawner.generated.h"

class IP12Interactable;
class AP12AICharacter;

UCLASS()
class GAMESANDBOX_API AP12CharacterAISpawner : public AActor
{
	GENERATED_BODY()

public:
	AP12CharacterAISpawner();
	UFUNCTION()
	void SpawnAI();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	void UnsubscribeFromTrigger();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	TSubclassOf<AP12AICharacter> CharacterClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	bool bSpawnOnStart = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	bool bDoOnce = false;

	/* An actor implementing IP12Interactable interface */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	AActor* SpawnTriggerActor;	
	
private:
	bool bCanSpawn = true;

	TScriptInterface<IP12Interactable> SpawnTrigger;
	FDelegateHandle TriggerDelegate;
};
