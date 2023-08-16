// Copyright (C) 2023, IKinder

#include "P9/Public/Prop/P9ToggleProp.h"

#include "EngineUtils.h"
#include "P9/Public/Prop/P9InteractedProp.h"

AP9ToggleProp::AP9ToggleProp()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP9ToggleProp::BeginPlay()
{
	Super::BeginPlay();
}

void AP9ToggleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP9ToggleProp::Interact(ACharacter* Causer)
{
	if (!Interfaces.IsEmpty())
	{
		for (const FName ToggleTag : ToggleTags)
		{
			for (IP9Interaction* Interface : Interfaces)
			{
				if (!Cast<AActor>(Interface)->ActorHasTag(ToggleTag)) continue;
				Interface->Interact(Causer);
			}
		}
		return;
	}
	
	for(TActorIterator<AActor> ActorIterator(GetWorld()); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		if (!Actor) continue;
		IP9Interaction* Interface = Cast<IP9Interaction>(Actor);
		if (!Interface || Interface == this ) continue;
		Interfaces.Add(Interface);
	}
}
