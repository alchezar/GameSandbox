// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Util/P16Type.h"
#include "P16AbilitySystemLibrary.generated.h"

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

private:
	static FP16WidgetControllerParams GetWidgetControllerParams(const UObject* WorldContextObject);
	static void                       ApplyGameplayEffect(UAbilitySystemComponent* AbilitySystemComponent, const TSubclassOf<UGameplayEffect>& GameplayEffect, const float Level, const AActor* SourceObject);
};
