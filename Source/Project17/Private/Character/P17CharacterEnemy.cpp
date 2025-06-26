// Copyright © 2025, Ivan Kinder

#include "Character/P17CharacterEnemy.h"

#include "Component/Combat/P17CombatEnemyComponent.h"
#include "Data/P17Data_StartupBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AP17CharacterEnemy::AP17CharacterEnemy()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (auto* Movement = GetCharacterMovement())
	{
		Movement->bUseControllerDesiredRotation = false;
		Movement->bOrientRotationToMovement = true;
		Movement->RotationRate = {0.f, 180.f, 0.f};
		Movement->MaxWalkSpeed = 300.f;
		Movement->BrakingDecelerationWalking = 1000.f;
	}

	CombatComponent = CreateDefaultSubobject<UP17CombatEnemyComponent>("EnemyCombatComponent");
}

void AP17CharacterEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Load startup data asynchronously.
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartupData.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::InitEnemyStartupData));
}

void AP17CharacterEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AP17CharacterEnemy::InitEnemyStartupData() const
{
	UP17Data_StartupBase* LoadedData = StartupData.Get();
	WARN_RETURN_IF(!LoadedData,)
	LoadedData->GiveToAbilitySystemComponent(GetProjectAbilitySystemComponent());
}
