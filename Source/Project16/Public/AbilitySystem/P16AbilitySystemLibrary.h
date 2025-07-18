// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Util/P16Type.h"
#include "P16AbilitySystemLibrary.generated.h"

class UP16LootTiersDataAsset;
class UP16SaveGame;
class UP16AbilityInfoDataAsset;
class AP16HUD;
class UP16SpellMenuWidgetController;
class UP16CharacterClassInfoDataAsset;
class UP16AttributeMenuWidgetController;
class UP16OverlayWidgetController;

UCLASS()
class PROJECT16_API UP16AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	///
	/// @name Widget controller.
	///
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DefaultToSelf = "WorldContextObject"))
	static UP16OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DefaultToSelf = "WorldContextObject"))
	static UP16AttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DefaultToSelf = "WorldContextObject"))
	static UP16SpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	///
	/// @name Ability system.
	///
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void InitDefaultAttributes(const UObject* WorldContextObject, const EP16CharacterClass CharacterClass, UAbilitySystemComponent* AbilitySystemComponent, const float Level);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void InitDefaultAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, UP16SaveGame* SaveGame);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, const EP16CharacterClass CharacterClass);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static UP16CharacterClassInfoDataAsset* GetCharacterClassInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static UP16AbilityInfoDataAsset* GetAbilityInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (DefaultToSelf = "WorldContextObject"))
	static UP16LootTiersDataAsset* GetLootTiersInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "C++")
	///
	/// @name Effect context getters.
	///
	static bool GetIsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static FP16DebuffSpec GetDebuffSpec(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static FGameplayTag GetDebuffDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static FP16RadialDamageParams GetRadialDamageParams(const FGameplayEffectContextHandle& EffectContextHandle);
	///
	/// @name Effect context setters.
	///
	// clang-format off
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bNewBlocked);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bNewCritical);
	UFUNCTION(BlueprintPure, Category = "C++")
	static void SetDebuffSpec(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bSuccessful, const FP16DebuffInfo& InDebuffInfo, const FGameplayTag& InDamageType);
	UFUNCTION(BlueprintPure, Category = "C++")
	static void SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);
	UFUNCTION(BlueprintPure, Category = "C++")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);
	UFUNCTION(BlueprintPure, Category = "C++")
	static void SetRadialDamageParams(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FP16RadialDamageParams& RadialDamageParams);
	// clang-format on
	///
	/// @name Gameplay mechanics.
	///
	UFUNCTION(BlueprintCallable, Category = "C++")
	static TArray<AActor*> GetLivePlayersWithinRadius(const UObject* WorldContextObject, const TArray<AActor*> IgnoredActors, const float Radius, const FVector SphereOrigin);
	UFUNCTION(BlueprintPure, Category = "C++")
	static bool GetIsNotFriends(const AActor* First, const AActor* Second);
	UFUNCTION(BlueprintPure, Category = "C++")
	static int32 GetXPRewardFor(const UObject* WorldContextObject, const EP16CharacterClass CharacterClass, const int32 Level);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FP16DamageEffectParams& Params);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& RotationAxis, const float SpreadAngle, const float Count);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static TArray<FVector> EvenlySpacedVectors(const FVector& Forward, const FVector& Axis, const float SpreadAngle, const float Count);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static TArray<AActor*> GetClosestTargets(const int32 MaxTargets, TArray<AActor*> Actors, const FVector& Origin);

private:
	static auto GetWidgetControllerParams(const UObject* WorldContextObject, AP16HUD** OutHUD = nullptr) -> FP16WidgetControllerParams;
	static void ApplyGameplayEffect(UAbilitySystemComponent* AbilitySystemComponent, const TSubclassOf<UGameplayEffect>& GameplayEffect, const float Level, const AActor* SourceObject);
};
