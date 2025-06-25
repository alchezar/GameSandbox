// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P17FunctionLibrary.generated.h"

class UP17AbilitySystemComponent;

UENUM()
enum class EP17ConfirmTypePin : uint8
{
	Yes,
	No
};

UCLASS()
class PROJECT17_API UP17FunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UP17AbilitySystemComponent* NativeGetASCFromActor(AActor* InActor);
	static bool NativeGetActorHasTag(AActor* InActor, const FGameplayTag InTag);

public:
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, const FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, const FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "P17 | FunctionLibrary", meta = (DisplayName = "GetActorHasTag", ExpandEnumAsExecs = "OutExecs"))
	static void BP_GetActorHasTag(AActor* InActor, const FGameplayTag InTag, EP17ConfirmTypePin& OutExecs);
};
