// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16Tags.h"
#include "UObject/Object.h"
#include "GSGameplayTagsSingleton.generated.h"

///
/// @class   FGSGameplayTagsSingleton
/// @brief   Represents a singleton class for storing gameplay tags.
/// @details Instead of getting gameplay tags from FGameplayTag::RequestGameplayTag, we can get them from this class.
///
USTRUCT()
struct GAMESANDBOX_API FGSGameplayTagsSingleton
{
	GENERATED_BODY()

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD static const FGSGameplayTagsSingleton& Get() { return Singleton; }

	static void InitializeNativeGameplayTags();

private:
	static void InitializeProject16GameplayTags();

	/* ------------------------------ Fields ------------------------------- */
public:
	FP16Tags P16Tags;

private:
	static FGSGameplayTagsSingleton Singleton;
};
