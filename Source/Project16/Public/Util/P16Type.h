// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/Widget/P16Widget.h"
#include "P16Type.generated.h"

class AP16PlayerController;
class AP16PlayerState;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UInputAction;
class UP16AbilitySystemComponent;
class UP16AttributeSet;

UENUM()
enum class EP16EffectApplicationPolicy : uint8
{
	// clang-format off
	None = 0       UMETA(DisplayName = "Do not apply"),
	OnBeginOverlap UMETA(DisplayName = "Apply on begin overlap"),
	OnEndOverlap   UMETA(DisplayName = "Apply on end overlap"),
	// clang-format on
};

UENUM()
enum class EP16EffectRemovalPolicy : uint8
{
	// clang-format off
	None = 0     UMETA(DisplayName = "Do not remove"),
	OnEndOverlap UMETA(DisplayName = "Remove on end overlap"),
	// clang-format on
};

UENUM(BlueprintType)
enum class EP16CharacterClass : uint8
{
	Elemental = 0,
	Warrior,
	Ranger
};

UENUM()
enum class EP16SaveSlotStatus : uint8
{
	Vacant = 0,
	Named,
	Taken
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

USTRUCT(BlueprintType)
struct FP16WidgetControllerSpecificParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AP16PlayerController> PlayerController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AP16PlayerState> PlayerState = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UP16AbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UP16AttributeSet> AttributeSet = nullptr;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat XPReward = {};
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
	FGameplayTag CooldownTag = FGameplayTag {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag TypeTag = FGameplayTag {};
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag {};
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<const UMaterialInterface> Background = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility> Ability = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 LevelRequirement = 1;
};

USTRUCT(BlueprintType)
struct FP16LevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

USTRUCT(BlueprintType)
struct FP16DamageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag = FGameplayTag {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat Damage = {};
};

/// @brief All information about debuff, encapsulated in one struct.
/// @details This struct designed to store on the source side all the information
/// about the debuffing we need to use. With the expectation of one debuff per ability.
USTRUCT(BlueprintType)
struct FP16DebuffInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Chance = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Frequency = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration = 5.f;
};

/// @brief All information about debuff, after trying to use it.
/// @details This is specified version of `FP16DebuffInfo` struct.
/// Designed to store on the target side all the information about the debuffing
/// after successful chance to use it.
USTRUCT(BlueprintType)
struct FP16DebuffSpec
{
	GENERATED_BODY()

	bool  bSuccessful = false;
	float Damage      = 0.f;
	float Frequency   = 0.f;
	float Duration    = 0.f;

	TSharedPtr<FGameplayTag> DamageType = {};

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar
			<< bSuccessful
			<< Damage
			<< Frequency
			<< Duration;

		if (Ar.IsLoading() && !DamageType.IsValid())
		{
			DamageType = MakeShared<FGameplayTag>();
		}
		return DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
};

USTRUCT(BlueprintType)
struct FP16TrickImpulse
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Magnitude = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (UIMin = 0.f, ClampMin = 0.f, UIMax = 100.f, ClampMax = 100.f))
	float Chance = 100.f;
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FP16RadialDamageParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bRadial = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InnerRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float OuterRadius = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Origin = {};

	friend FArchive& operator<<(FArchive& Ar, FP16RadialDamageParams& DamageParams)
	{
		Ar
			<< DamageParams.bRadial
			<< DamageParams.InnerRadius
			<< DamageParams.OuterRadius
			<< DamageParams.Origin;

		return Ar;
	}
};

USTRUCT(BlueprintType)
struct FP16DamageEffectParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WorldContext = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadOnly)
	float BaseDamage = 0.f;
	UPROPERTY(BlueprintReadOnly)
	int32 AbilityLevel = 1;
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag DamageType = {};
	UPROPERTY(BlueprintReadOnly)
	FP16DebuffInfo Debuff = {};
	UPROPERTY(BlueprintReadWrite)
	FP16TrickImpulse DeathImpulse = {};
	UPROPERTY(BlueprintReadWrite)
	FP16TrickImpulse Knockback = {};
	UPROPERTY(BlueprintReadWrite)
	FP16RadialDamageParams RadialParams = {};
};

struct FP16SelectedAbility
{
	FGameplayTag Ability     = {};
	FGameplayTag Status      = {};
	int32        SpellPoints = 0;
};

struct FP16AbilityDescription
{
	FString CurrentLevel = {};
	FString NextLevel    = {};
};

template <typename T>
concept RangeComparable = requires(T Min, T Max)
{
	{ Min <= Max } -> std::convertible_to<bool>;
	{ Min > Max } -> std::convertible_to<bool>;
};

template <RangeComparable T>
struct TP16Range
{
	T Min;
	T Max;
};
