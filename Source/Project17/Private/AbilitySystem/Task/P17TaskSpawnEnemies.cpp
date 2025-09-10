// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Task/P17TaskSpawnEnemies.h"

#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
#include "Project17.h"
#include "Character/P17CharacterEnemy.h"
#include "Engine/AssetManager.h"

void UP17TaskSpawnEnemies::Activate()
{
	// Listen for gameplay event.
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	DelegateHandle = Delegate.AddUObject(this, &ThisClass::OnGameplayEventReceivedCallback);
}

void UP17TaskSpawnEnemies::OnDestroy(const bool bInOwnerFinished)
{
	// Stop listening for gameplay event.
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	Delegate.Remove(DelegateHandle);

	Super::OnDestroy(bInOwnerFinished);
}

UP17TaskSpawnEnemies* UP17TaskSpawnEnemies::CreateTaskSpawnEnemies(
	UGameplayAbility* OwningAbility,
	const FGameplayTag EventTag,
	const TSoftClassPtr<AP17CharacterEnemy> SoftEnemyClassToSpawn,
	const FVector& SpawnOrigin,
	const int32 NumToSpawn,
	const float RandomSpawnRadius)
{
	UP17TaskSpawnEnemies* Node = NewAbilityTask<UP17TaskSpawnEnemies>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;

	return Node;
}

void UP17TaskSpawnEnemies::OnGameplayEventReceivedCallback(const FGameplayEventData* Payload)
{
	if (const bool bInvalidSoftClass = CachedSoftEnemyClassToSpawn.IsNull())
	{
		LOUD_WARN(bInvalidSoftClass)
		FinishSpawnTask();
		return;
	}

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		CachedSoftEnemyClassToSpawn.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoadedCallback));
}

void UP17TaskSpawnEnemies::OnEnemyClassLoadedCallback()
{
	WARN_RETURN_IF(!GetWorld(),)
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	if (!LoadedClass)
	{
		LOUD_WARN(!LoadedClass)
		FinishSpawnTask();
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<AP17CharacterEnemy*> SpawnedEnemies = {};
	for (int32 i = 0; i < CachedNumToSpawn; ++i)
	{
		AP17CharacterEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AP17CharacterEnemy>(
			LoadedClass,
			GetRandomSpawnLocation(),
			GetSpawnRotation(),
			SpawnParameters);

		CONTINUE_IF(!SpawnedEnemy)
		SpawnedEnemies.Add(SpawnedEnemy);
	}

	FinishSpawnTask(SpawnedEnemies);
}

void UP17TaskSpawnEnemies::FinishSpawnTask(const TArray<AP17CharacterEnemy*>& SpawnedEnemies)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		if (SpawnedEnemies.IsEmpty())
		{
			DidNotSpawn.Broadcast(SpawnedEnemies);
		}
		else
		{
			OnSpawnFinished.Broadcast(SpawnedEnemies);
		}
	}

	EndTask();
}

FVector UP17TaskSpawnEnemies::GetRandomSpawnLocation(const float HeightOffset)
{
	FVector SpawnLocation = {};
	UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this, CachedSpawnOrigin, SpawnLocation, CachedRandomSpawnRadius);
	SpawnLocation += FVector::UpVector * HeightOffset;

	return SpawnLocation;
}

FRotator UP17TaskSpawnEnemies::GetSpawnRotation() const
{
	return AbilitySystemComponent->GetAvatarActor()->GetActorRotation();
}
