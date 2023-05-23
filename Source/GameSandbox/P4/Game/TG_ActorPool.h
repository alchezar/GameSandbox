// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TG_ActorPool.generated.h"

class UTG_ActorPool;
class ANavMeshBoundsVolume;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UTG_ActorPool : public UActorComponent
{
	GENERATED_BODY()

public:
	UTG_ActorPool();

	ANavMeshBoundsVolume* Checkout();
	void Return(ANavMeshBoundsVolume* ActorToReturn);
	void Add(ANavMeshBoundsVolume* ActorToAdd);

protected:
	virtual void BeginPlay() override;

private:
	TArray<ANavMeshBoundsVolume*> Pool;
};
