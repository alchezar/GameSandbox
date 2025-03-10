// Copyright © 2025, Ivan Kinder

#include "Root/Public/Singleton/GSAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

UGSAssetManager& UGSAssetManager::Get()
{
	check(GEngine)
	UGSAssetManager* AssetManager = Cast<UGSAssetManager>(GEngine->AssetManager);
	check(AssetManager)

	return *AssetManager;
}

void UGSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// Appropriate place for initialization of all custom gameplay tags.
	FGSGameplayTagsSingleton::InitializeNativeGameplayTags();

	// This is required to use the target data (initializing the target data cache).
	UAbilitySystemGlobals::Get().InitGlobalData();
}
