// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GSAssetManager.generated.h"

///
/// @class   UGSAssetManager
/// @brief   Represents a custom asset manager class.
/// @details Extends the base class with the ability to initialize custom gameplay tags.
///
UCLASS()
class GAMESANDBOX_API UGSAssetManager : public UAssetManager
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	/// @brief Returns the current AssetManager object.
	static UGSAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
