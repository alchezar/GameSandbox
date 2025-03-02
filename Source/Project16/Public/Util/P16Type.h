// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/Widget/P16Widget.h"
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

USTRUCT()
struct FP16EffectProperties
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayEffectContextHandle EffectContext = {};
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystem = nullptr;
	UPROPERTY()
	TObjectPtr<AActor> SourceAvatarActor = nullptr;
	UPROPERTY()
	TObjectPtr<AController> SourceController = nullptr;
	UPROPERTY()
	TObjectPtr<ACharacter> SourceCharacter = nullptr;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystem = nullptr;
	UPROPERTY()
	TObjectPtr<AActor> TargetAvatarActor = nullptr;
	UPROPERTY()
	TObjectPtr<AController> TargetController = nullptr;
	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter = nullptr;
};

USTRUCT(BlueprintType)
struct FP16UIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag MessageTag = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UP16Widget> MessageWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image = nullptr;
};
