// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project16.h"
#include "AbilitySystem/Ability/P16GameplayAbility.h"
#include "Interface/P16PlayerInterface.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"
#include "Util/P16Log.h"

UP16AbilitySystemComponent::UP16AbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP16AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP16AbilitySystemComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP16AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	// Broadcast abilities given event on clients.
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		OnAbilitiesGiven.Broadcast(this);
	}
}

FGameplayTag UP16AbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& InAbilitySpec)
{
	EARLY_RETURN_VALUE_IF(!InAbilitySpec.Ability, {});

	const FGameplayTag* FoundTag = InAbilitySpec
		.Ability->GetAssetTags()
		.GetGameplayTagArray()
		.FindByPredicate([](const FGameplayTag Tag) -> bool
		{
			return Tag.MatchesTag(FGSGameplayTagsSingleton::Get().P16Tags.Ability.Tag);
		});

	return FoundTag ? *FoundTag : FGameplayTag {};
}

FGameplayTag UP16AbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& InAbilitySpec)
{
	const auto* FoundTag = InAbilitySpec
		.GetDynamicSpecSourceTags()
		.GetGameplayTagArray()
		.FindByPredicate([](const FGameplayTag Tag) -> bool
		{
			return Tag.MatchesTag(FGSGameplayTagsSingleton::Get().P16Tags.Input.Tag);
		});

	return FoundTag ? *FoundTag : FGameplayTag {};
}

void UP16AbilitySystemComponent::OnAbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::Client_OnEffectAppliedCallback);
}

void UP16AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UP16GameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UP16GameplayAbility>& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec       AbilitySpec = {AbilityClass};
		const UP16GameplayAbility* Ability     = Cast<UP16GameplayAbility>(AbilitySpec.Ability);
		CONTINUE_IF(!Ability)

		AbilitySpec.GetDynamicSpecSourceTags().AddTag(Ability->StartupInputTag);
		GiveAbility(AbilitySpec);
	}

	bStartupAbilitiesGiven = true;
	OnAbilitiesGiven.Broadcast(this);
}

void UP16AbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& PassiveAbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec PassiveAbilitySpec = {PassiveAbilityClass};
		GiveAbilityAndActivateOnce(PassiveAbilitySpec);
	}
}

void UP16AbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))

		AbilitySpecInputPressed(AbilitySpec);
		CONTINUE_IF(AbilitySpec.IsActive())

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UP16AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))

		AbilitySpecInputReleased(AbilitySpec);
	}
}

void UP16AbilitySystemComponent::ForEachAbility(const FP16ForEachAbilitySignature& InDelegate)
{
	FScopedAbilityListLock ActiveScopeLock {*this};
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (!InDelegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogP16, Warning, L"Failed to execute delegate in %hs.", __FUNCTION__);
		}
	}
}

void UP16AbilitySystemComponent::UpdateAttribute(const FGameplayTag& AttributeTag)
{
	EARLY_RETURN_IF(GetAvatarActor()->Implements<UP16PlayerInterface>()
		&& IP16PlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) == 0)

	Server_UpdateAttribute(AttributeTag);
}

void UP16AbilitySystemComponent::Client_OnEffectAppliedCallback_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	GameplayEffectSpec.GetAllAssetTags(TagContainer);

	OnEffectApplied.Broadcast(TagContainer);
}

void UP16AbilitySystemComponent::Server_UpdateAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag       = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, MoveTemp(Payload));

	IP16PlayerInterface::Execute_AddAttributePoints(GetAvatarActor(), -1);
}
