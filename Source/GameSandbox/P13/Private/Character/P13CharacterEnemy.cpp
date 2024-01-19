// Copyright © 2024, IKinder

#include "P13/Public/Character/P13CharacterEnemy.h"

#include "P13/Public/Component/Actor/P13ScoreComponent.h"

AP13CharacterEnemy::AP13CharacterEnemy()
{
	CreateEnemyComponents();
}

void AP13CharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
	UpdateInventoryAfterRespawn();
}

void AP13CharacterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13CharacterEnemy::CreateEnemyComponents()
{
	ScoreComp = CreateDefaultSubobject<UP13ScoreComponent>("EnemyScoreComponent");
}
