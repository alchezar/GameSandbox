// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13EnemySpawn.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "P13/Public/Character/P13CharacterEnemy.h"
#include "P13/Public/Game/P13GameState.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             Enemy spawn point                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

AP13EnemySpawnPoint::AP13EnemySpawnPoint() {}

void AP13EnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	CachedGameState = GetWorld()->GetGameState<AP13GameState>();
	check(CachedGameState)
}

bool AP13EnemySpawnPoint::TrySpawnEnemy()
{
	/* Find last level phase. */
	if (Phase != CachedGameState->GetLastLevelPhase())
	{
		return false;
	}
	SpawnEnemy();
	return true;
}

void AP13EnemySpawnPoint::SpawnEnemy()
{
	const FTransform PointTransform = GetActorTransform();
	SpawnEnemyAt(PointTransform);
}

void AP13EnemySpawnPoint::SpawnEnemyAt(const FTransform& SpawnTransform) const
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	const AP13CharacterEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AP13CharacterEnemy>(EnemyClass, SpawnTransform, SpawnParams);
	check(SpawnedEnemy)
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Enemy spawn box                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

AP13EnemySpawnBox::AP13EnemySpawnBox()
{
	SpawnBox = CreateDefaultSubobject<UBoxComponent>("SpawnBoxComponent");
	SpawnBox->SetupAttachment(RootComponent);
	SpawnBox->SetCollisionProfileName("NoCollision");
}

void AP13EnemySpawnBox::SpawnEnemy()
{
	const FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox_Box(SpawnBox->Bounds.GetBox());
	const FRotator SpawnRotation = FRotator(0.f, FMath::RandRange(0.f, 360.f), 0.f);

	SpawnEnemyAt(FTransform(SpawnRotation, SpawnLocation));
}
