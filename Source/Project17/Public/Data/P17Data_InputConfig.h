// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "P17Data_InputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(BlueprintType)
struct FP17InputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag Tag = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> Action = nullptr;
};

/// ----------------------------------------------------------------------------
/// @class   UP17Data_InputConfig
/// @brief   Represents a data asset for input configuration.
/// @details Extends the base class with an ability to store and find native
/// input actions.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT17_API UP17Data_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	_NODISCARD
	UInputMappingContext* GetInputMappingContext() { return DefaultContext; }

	UInputAction* FindNativeInputAction(const FGameplayTag& InInputTag) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TObjectPtr<UInputMappingContext> DefaultContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "Tag"));
	TArray<FP17InputActionConfig> NativeInputActions = {};
};
