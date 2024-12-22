// Copyright (C) 2023, IKinder

#include "TG_ActorPool.h"

#include "NavMesh/NavMeshBoundsVolume.h"

UTG_ActorPool::UTG_ActorPool()
{
}

void UTG_ActorPool::BeginPlay()
{
	Super::BeginPlay();
}

ANavMeshBoundsVolume* UTG_ActorPool::Checkout()
{
	return Pool.IsEmpty() ? nullptr : Pool.Pop();
}

void UTG_ActorPool::Return(ANavMeshBoundsVolume* ActorToReturn)
{
	Add(ActorToReturn);
}

void UTG_ActorPool::Add(ANavMeshBoundsVolume* ActorToAdd)
{
	if (!ActorToAdd) return;

	Pool.Push(ActorToAdd);
}
