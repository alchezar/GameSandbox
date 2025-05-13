// Copyright © 2025, Ivan Kinder

#include "Actor/P16SpawnPoint.h"

#include "Project16.h"
#include "Character/P16Enemy.h"
#include "Util/P16Log.h"

AP16SpawnPoint::AP16SpawnPoint()
{}

void AP16SpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AP16SpawnPoint::Spawn()
{
	// Spawn one random enemy from the array.
	SpawnOneEnemy(GetRandomSpawnInfo(), GetActorTransform());
}

FP16EnemyInfo AP16SpawnPoint::GetRandomSpawnInfo()
{
	const int32 Num = EnemySpawnInfos.Num();

	if (Num == 0)
	{
		return FP16EnemyInfo {};
	}
	if (Num == 1)
	{
		return EnemySpawnInfos[0];
	}

	const int32 Rand = FMath::RandRange(0, Num - 1);
	return EnemySpawnInfos[Rand];
}

void AP16SpawnPoint::SpawnOneEnemy(const FP16EnemyInfo& EnemyInfo, const FTransform& Transform) const
{
	EARLY_RETURN_WITH_WARN_IF(!EnemyInfo.Class);

	AP16Enemy* Enemy = GetWorld()->SpawnActorDeferred<AP16Enemy>(
		EnemyInfo.Class,
		Transform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	EARLY_RETURN_WITH_WARN_IF(!Enemy)
	Enemy->SetLevel(EnemyInfo.Level);
	Enemy->FinishSpawning(Transform);
	Enemy->SpawnDefaultController();
}
