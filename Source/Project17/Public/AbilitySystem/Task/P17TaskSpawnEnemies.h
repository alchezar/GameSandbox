// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "P17TaskSpawnEnemies.generated.h"

class AP17CharacterEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP17WaitSpawnEnemiesDelegate, const TArray<AP17CharacterEnemy*>&, SpawnedEnemies);

UCLASS()
class PROJECT17_API UP17TaskSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
protected:
	/// @par UGameplayTask interface -------------------------------------------
	virtual void Activate() override;
	virtual void OnDestroy(const bool bInOwnerFinished) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (DisplayName = "Wait gameplay event and spawn enemies", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", NumToSpawn = "1", RandomSpawnRadius = "200"))
	static UP17TaskSpawnEnemies* CreateTaskSpawnEnemies(
		UGameplayAbility* OwningAbility,
		const FGameplayTag EventTag,
		const TSoftClassPtr<AP17CharacterEnemy> SoftEnemyClassToSpawn,
		const FVector& SpawnOrigin,
		const int32 NumToSpawn = 1,
		const float RandomSpawnRadius = 200.f);

protected:
	void OnGameplayEventReceivedCallback(const FGameplayEventData* Payload);
	void OnEnemyClassLoadedCallback();

private:
	void FinishSpawnTask(const TArray<AP17CharacterEnemy*>& SpawnedEnemies = {});
	FVector GetRandomSpawnLocation(const float HeightOffset = 150.f);
	FRotator GetSpawnRotation() const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP17WaitSpawnEnemiesDelegate OnSpawnFinished;
	UPROPERTY(BlueprintAssignable)
	FP17WaitSpawnEnemiesDelegate DidNotSpawn;

private:
	FGameplayTag CachedEventTag = {};
	TSoftClassPtr<AP17CharacterEnemy> CachedSoftEnemyClassToSpawn = {};
	FVector CachedSpawnOrigin = {};
	int32 CachedNumToSpawn = 1;
	float CachedRandomSpawnRadius = 200.f;

	FDelegateHandle DelegateHandle;
};
