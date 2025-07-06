// Copyright © 2025, Ivan Kinder

#include "Util/P17FunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "Interface/P17CombatInterface.h"

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

UP17CombatPawnComponent* UP17FunctionLibrary::NativeGetCombatComponentFromActor(AActor* InActor)
{
	WARN_RETURN_IF(!InActor, nullptr)

	const TScriptInterface<IP17CombatInterface> CombatInterface = InActor;
	WARN_RETURN_IF(!CombatInterface, nullptr)

	return CombatInterface->GetCombatComponent();
}

void UP17FunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, const FGameplayTag InTag)
{
	UP17AbilitySystemComponent* ASC = NativeGetASCFromActor(InActor);
	WARN_RETURN_IF(!ASC,)
	RETURN_IF(ASC->HasMatchingGameplayTag(InTag),)

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

UP17CombatPawnComponent* UP17FunctionLibrary::BP_GetCombatComponentFromActor(AActor* InActor, EP17ValidTypePin& OutExecs)
{
	UP17CombatPawnComponent* CombatComponent = NativeGetCombatComponentFromActor(InActor);
	OutExecs = CombatComponent ? EP17ValidTypePin::Valid : EP17ValidTypePin::Invalid;
	return CombatComponent;
}

bool UP17FunctionLibrary::IsTargetHostile(const APawn* InQuery, const APawn* InTarget)
{
	RETURN_IF(!InQuery || !InTarget, false);
	const auto* QueryTeamAgent = InQuery->GetController<IGenericTeamAgentInterface>();
	const auto* TargetTeamAgent = InTarget->GetController<IGenericTeamAgentInterface>();
	RETURN_IF(!QueryTeamAgent || !TargetTeamAgent, false);

	return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
}

float UP17FunctionLibrary::BP_GetValueAtLevel(const FScalableFloat& InScalableFloat, const int32 InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FName UP17FunctionLibrary::ComputeHitReactDirection(const AActor* InAttacker, const AActor* InVictim)
{
	WARN_RETURN_IF(!InAttacker || !InVictim, "None")

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttacker = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal2D();
	const float DotProduct = VictimForward | VictimToAttacker;
	const float RightSide = FMath::Sign((VictimForward ^ VictimToAttacker).Z);

	// clang-format off
	return DotProduct > 0.5f ? "Front" :
		DotProduct < -0.5f ? "Back" :
		RightSide > 0.f ? "Right" : "Left";
	// clang-format on
}
