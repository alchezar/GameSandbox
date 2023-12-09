// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Spawner/P12CharacterAISpawner.h"

#include "P12/Public/Actor/Interactive/Interface/P12Interactable.h"
#include "P12/Public/AI/Character/P12AICharacter.h"

AP12CharacterAISpawner::AP12CharacterAISpawner()
{
	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	SetRootComponent(SceneRoot);
}

void AP12CharacterAISpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnTrigger = SpawnTriggerActor;
	if (SpawnTrigger.GetInterface())
	{
		TriggerDelegate = SpawnTrigger->AddOnInteractionFunction(this, "SpawnAI");
	}

	if (bSpawnOnStart)
	{
		SpawnAI();
	}
}

void AP12CharacterAISpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// UnsubscribeFromTrigger();

	Super::EndPlay(EndPlayReason);
}

void AP12CharacterAISpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	/* This is the way how to filter picking actors by interface in editor. */
	if (PropertyChangedEvent.Property->GetName() == GET_MEMBER_NAME_STRING_CHECKED(ThisClass, SpawnTriggerActor))
	{
		SpawnTrigger = SpawnTriggerActor;
		if (SpawnTrigger.GetInterface() && SpawnTrigger->HasOnInteractionCallback())
		{
			return;
		}
		SpawnTriggerActor = nullptr;
		SpawnTrigger = nullptr;
	}
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
		UnsubscribeFromTrigger();
		bCanSpawn = false;
	}
}

void AP12CharacterAISpawner::UnsubscribeFromTrigger()
{
	if (!SpawnTrigger.GetInterface() || !TriggerDelegate.IsValid())
	{
		return;
	}
	SpawnTrigger->RemoveOnInteractionDelegate(TriggerDelegate);
}
