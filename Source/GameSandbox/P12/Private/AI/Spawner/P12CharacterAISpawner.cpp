// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Spawner/P12CharacterAISpawner.h"

#include "P12/Public/AI/Character/P12AICharacter.h"

AP12CharacterAISpawner::AP12CharacterAISpawner()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);
}

void AP12CharacterAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bSpawnOnStart)
	{
		SpawnAI();
	}
}

void AP12CharacterAISpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AP12CharacterAISpawner::SpawnAI()
{
	if (!bCanSpawn || !CharacterClass)
	{
		return;
	}
	AP12AICharacter* Character = GetWorld()->SpawnActor<AP12AICharacter>(CharacterClass, GetTransform());
	check(Character)
	if (!Character->Controller)
	{
		Character->SpawnDefaultController();
	}
	if (bDoOnce)
	{
		bCanSpawn = false;
	}
}
