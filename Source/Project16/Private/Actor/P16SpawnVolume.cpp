// Copyright © 2025, Ivan Kinder

#include "Actor/P16SpawnVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

AP16SpawnVolume::AP16SpawnVolume()
{
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>("SpawnVolumeBoxComponent");
	SpawnVolume->SetupAttachment(GetRootComponent());
}

void AP16SpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

void AP16SpawnVolume::Spawn()
{
	// Spawn each enemy from the array in the volume.
	for (const FP16EnemyInfo& Info : EnemySpawnInfos)
	{
		FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(
			SpawnVolume->Bounds.Origin,
			SpawnVolume->Bounds.BoxExtent);
		RandomLocation.Z = GetActorLocation().Z;

		SpawnOneEnemy(Info, {GetActorRotation(), RandomLocation});
	}
}
