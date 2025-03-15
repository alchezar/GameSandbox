// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Util/P16Type.h"
#include "P16AbilitySystemLibrary.generated.h"

class UP16CharacterClassInfoDataAsset;
class UP16AttributeMenuWidgetController;
class UP16OverlayWidgetController;

UCLASS()
class PROJECT16_API UP16AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "C++")
	static UP16OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "C++")
	static UP16AttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void InitDefaultAttributes(const UObject* WorldContextObject, const EP16CharacterClass CharacterClass, UAbilitySystemComponent* AbilitySystemComponent, const float Level);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static UP16CharacterClassInfoDataAsset* GetCharacterClassInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "C++")
	static bool GetIsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintPure, Category = "C++")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bNewBlocked);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bNewCritical);

private:
	static auto GetWidgetControllerParams(const UObject* WorldContextObject) -> FP16WidgetControllerParams;
	static void ApplyGameplayEffect(UAbilitySystemComponent* AbilitySystemComponent, const TSubclassOf<UGameplayEffect>& GameplayEffect, const float Level, const AActor* SourceObject);
};
