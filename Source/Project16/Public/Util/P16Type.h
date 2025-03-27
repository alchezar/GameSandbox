// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/Widget/P16Widget.h"
#include "P16Type.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UInputAction;

UENUM()
enum class EP16EffectApplicationPolicy : uint8
{
	None = 0 UMETA(DisplayName = "Do not apply"),
	OnBeginOverlap UMETA(DisplayName = "Apply on begin overlap"),
	OnEndOverlap   UMETA(DisplayName = "Apply on end overlap"),
};

UENUM()
enum class EP16EffectRemovalPolicy : uint8
{
	None = 0 UMETA(DisplayName = "Do not remove"),
	OnEndOverlap UMETA(DisplayName = "Remove on end overlap"),
};

UENUM(BlueprintType)
enum class EP16CharacterClass : uint8
{
	Elemental = 0,
	Warrior,
	Ranger
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

USTRUCT(BlueprintType)
struct FP16AttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Tag = FGameplayTag {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name = FText::GetEmpty();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description = FText::GetEmpty();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttribute Attribute = {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Value = 0.f;
};

USTRUCT(BlueprintType)
struct FP16InputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	const UInputAction* Action = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag Tag = FGameplayTag {};
};

USTRUCT(BlueprintType)
struct FP16CharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> PrimaryAttributes = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities = {};
};

USTRUCT(BlueprintType)
struct FP16TaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag Attack = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag Socket = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> Sound = nullptr;
};

USTRUCT(BlueprintType)
struct FP16AbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<const UMaterialInterface> Background = nullptr;

	FGameplayTag InputTag = FGameplayTag {};
};
