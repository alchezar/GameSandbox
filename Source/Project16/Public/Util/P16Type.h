// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P16Type.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UENUM()
enum class EP16EffectApplicationPolicy : uint8
{
	None = 0 UMETA(DisplayName = "Do not apply"),
	OnBeginOverlap UMETA(DisplayName = "Apply on begin overlap"),
	OnEndOverlap UMETA(DisplayName = "Apply on end overlap"),
};

UENUM()
enum class EP16EffectRemovalPolicy : uint8
{
	None = 0 UMETA(DisplayName = "Do not remove"),
	OnEndOverlap UMETA(DisplayName = "Remove on end overlap"),
};

USTRUCT(BlueprintType)
struct FP16WidgetControllerParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
