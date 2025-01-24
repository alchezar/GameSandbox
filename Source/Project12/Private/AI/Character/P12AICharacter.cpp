// Copyright (C) 2023, IKinder

#include "AI/Character/P12AICharacter.h"

#include "Component/Actor/P12AIPatrollingComponent.h"

AP12AICharacter::AP12AICharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AIPatrollingComponent = CreateDefaultSubobject<UP12AIPatrollingComponent>("AIPatrollingActorComponent");	
}
