// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P17Types.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P17FunctionLibrary.generated.h"

class UP17CombatPawnComponent;
class UP17AbilitySystemComponent;

UCLASS()
class PROJECT17_API UP17FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UP17AbilitySystemComponent* NativeGetASCFromActor(AActor* InActor);
	static bool NativeGetActorHasTag(AActor* InActor, const FGameplayTag InTag);
	static UP17CombatPawnComponent* NativeGetCombatComponentFromActor(AActor* InActor);

public:
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, const FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, const FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary", meta = (DisplayName = "GetActorHasTag", ExpandEnumAsExecs = "OutExecs"))
	static void BP_GetActorHasTag(AActor* InActor, const FGameplayTag InTag, EP17ConfirmTypePin& OutExecs);
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary", meta = (DisplayName = "GetCombatComponentFromActor", ExpandEnumAsExecs = "OutExecs"))
	static UP17CombatPawnComponent* BP_GetCombatComponentFromActor(AActor* InActor, EP17ValidTypePin& OutExecs);
	UFUNCTION(BlueprintPure, Category = "P17 | FunctionLibrary")
	static bool IsTargetHostile(const APawn* InQuery, const APawn* InTarget);
	UFUNCTION(BlueprintPure, Category = "P17 | FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float BP_GetValueAtLevel(const FScalableFloat& InScalableFloat, const int32 InLevel);
	UFUNCTION(BlueprintPure, Category = "P17 | FunctionLibrary", meta = (CompactNodeTitle = "Compute Hit React Direction Name"))
	static FName ComputeHitReactDirection(const AActor* InAttacker, const AActor* InVictim);
};
