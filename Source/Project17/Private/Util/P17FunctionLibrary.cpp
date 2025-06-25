// Copyright © 2025, Ivan Kinder

#include "Util/P17FunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"

UP17AbilitySystemComponent* UP17FunctionLibrary::NativeGetASCFromActor(AActor* InActor)
{
	WARN_RETURN_IF(!InActor, nullptr)
	WARN_RETURN_IF(!InActor->Implements<UAbilitySystemInterface>(), nullptr)

	return Cast<UP17AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

bool UP17FunctionLibrary::NativeGetActorHasTag(AActor* InActor, const FGameplayTag InTag)
{
	const UP17AbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);
	WARN_RETURN_IF(!ASC, false)

	return ASC->HasMatchingGameplayTag(InTag);
}

void UP17FunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, const FGameplayTag InTag)
{
	UP17AbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);
	WARN_RETURN_IF(!ASC,)
	WARN_RETURN_IF(ASC->HasMatchingGameplayTag(InTag),)

	ASC->AddLooseGameplayTag(InTag);
}

void UP17FunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, const FGameplayTag InTag)
{
	UP17AbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);
	WARN_RETURN_IF(!ASC,)
	RETURN_IF(!ASC->HasMatchingGameplayTag(InTag),)

	ASC->RemoveLooseGameplayTag(InTag);
}

void UP17FunctionLibrary::BP_GetActorHasTag(AActor* InActor, const FGameplayTag InTag, EP17ConfirmTypePin& OutExecs)
{
	OutExecs = NativeGetActorHasTag(InActor, InTag)
		? EP17ConfirmTypePin::Yes
		: EP17ConfirmTypePin::No;
}
