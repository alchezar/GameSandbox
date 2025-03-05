// Copyright © 2025, Ivan Kinder

#include "Root/Public/Singleton/GSAssetManager.h"

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
}
