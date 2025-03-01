// Copyright © 2025, Ivan Kinder

#include "Actor/P16EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Project16.h"

AP16EffectActor::AP16EffectActor()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->SetCollisionProfileName("NoCollision");
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Collision object will be setup in Blueprint.
}

void AP16EffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AP16EffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (ApplicationPolicy == EP16EffectApplicationPolicy::OnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}
}

void AP16EffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (ApplicationPolicy == EP16EffectApplicationPolicy::OnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, GameplayEffectClass);
	}
	else if (RemovalPolicy == EP16EffectRemovalPolicy::OnEndOverlap)
	{
		RemoveActiveGameplayEffect(TargetActor);
	}
}

void AP16EffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect>& InGameplayEffectClass)
{
	// Ignore actors without ability system component.
	UAbilitySystemComponent* TargetAbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	EARLY_RETURN_IF(!TargetAbilityComponent)
	check(InGameplayEffectClass)

	// Apply the effect.
	const FGameplayEffectContextHandle EffectContext = TargetAbilityComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle    EffectSpec    = TargetAbilityComponent->MakeOutgoingSpec(InGameplayEffectClass, EffectLevel, EffectContext);
	const FActiveGameplayEffectHandle  AppliedEffect = TargetAbilityComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

	// Store infinite effects for removal on end overlap.
	const bool bInfiniteEffect  = EffectSpec.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	const bool bRemovableEffect = RemovalPolicy == EP16EffectRemovalPolicy::OnEndOverlap;
	if (bInfiniteEffect && bRemovableEffect)
	{
		ActiveEffectsMap.Add(AppliedEffect, TargetAbilityComponent);
	}

	// Destroy not an infinite effect actor on successful apply.
	if (AppliedEffect.WasSuccessfullyApplied() && !bInfiniteEffect)
	{
		Destroy();
	}
}

void AP16EffectActor::RemoveActiveGameplayEffect(AActor* TargetActor)
{
	// Ignore actors without ability system component.
	const UAbilitySystemComponent* TargetAbilityComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	EARLY_RETURN_IF(!TargetAbilityComponent)

	// Remove infinite effects from ability system component.
	TArray<FActiveGameplayEffectHandle> EffectsToRemove = {};
	for (auto [Effect, AbilityComponent] : ActiveEffectsMap)
	{
		CONTINUE_IF(AbilityComponent != TargetAbilityComponent)
		AbilityComponent->RemoveActiveGameplayEffect(Effect, 1);
		EffectsToRemove.Add(Effect);
	}

	// Remove infinite effects from map.
	for (FActiveGameplayEffectHandle EffectToRemove : EffectsToRemove)
	{
		ActiveEffectsMap.FindAndRemoveChecked(EffectToRemove);
	}

	// Destroy infinite effect actor on successful removal.
	if (bDestroyOnEffectRemoval)
	{
		Destroy();
	}
}
