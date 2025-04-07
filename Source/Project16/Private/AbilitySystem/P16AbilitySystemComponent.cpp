// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/Ability/P16GameplayAbility.h"
#include "AbilitySystem/Data/P16AbilityInfoDataAsset.h"
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
		OnAbilitiesGiven.Broadcast();
	}
}

FGameplayAbilitySpec* UP16AbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& InTag)
{
	FScopedAbilityListLock Lock {*this};

	auto AnyOf = [InTag](const FGameplayTag& Tag) -> bool
	{
		return InTag.MatchesTag(Tag);
	};
	auto FindIf = [AnyOf](const FGameplayAbilitySpec& Spec) -> bool
	{
		return Spec.Ability->AbilityTags.GetGameplayTagArray().ContainsByPredicate(AnyOf);
	};

	return GetActivatableAbilities().FindByPredicate(FindIf);
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

FGameplayTag UP16AbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& InAbilitySpec)
{
	const auto FoundTag = InAbilitySpec
		.GetDynamicSpecSourceTags()
		.GetGameplayTagArray()
		.FindByPredicate([](const FGameplayTag Tag) -> bool
		{
			return Tag.MatchesTag(FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status.Tag);
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
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status.EquippedTag);
		GiveAbility(AbilitySpec);
	}

	bStartupAbilitiesGiven = true;
	OnAbilitiesGiven.Broadcast();
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
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
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

void UP16AbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
	UP16AbilityInfoDataAsset* AbilityInfo = UP16AbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FP16AbilityInfo& Info : AbilityInfo->AbilityInfos)
	{
		CONTINUE_IF(!Info.AbilityTag.IsValid()
			|| Level < Info.LevelRequirement
			|| GetSpecFromAbilityTag(Info.AbilityTag))

		constexpr int32      AbilityLevel = 1;
		FGameplayAbilitySpec AbilitySpec  = {Info.Ability, AbilityLevel};
		FGameplayTag         StatusTag    = FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status.EligibleTag;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(StatusTag);
		GiveAbility(AbilitySpec);
		MarkAbilitySpecDirty(AbilitySpec);
		Client_OnUpdateAbilityStatus(Info.AbilityTag, StatusTag, AbilityLevel);
	}
}

void UP16AbilitySystemComponent::Server_SpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag);
	EARLY_RETURN_IF(!Spec)

	const auto   AbilityTags = FGSGameplayTagsSingleton::Get().P16Tags.Ability;
	FGameplayTag Status      = GetStatusFromSpec(*Spec);
	if (Status.MatchesTagExact(AbilityTags.Status.EligibleTag))
	{
		// Change status.
		Spec->GetDynamicSpecSourceTags().RemoveTag(Status);
		Status = AbilityTags.Status.UnlockedTag;
		Spec->GetDynamicSpecSourceTags().AddTag(Status);
	}
	else if (Status.MatchesTagExact(AbilityTags.Status.EquippedTag)
		|| Status.MatchesTagExact(AbilityTags.Status.UnlockedTag))
	{
		// Just increase ability level.
		++Spec->Level;
	}

	// Spend points.
	if (GetAvatarActor()->Implements<UP16PlayerInterface>())
	{
		IP16PlayerInterface::Execute_AddSpellPoints(GetAvatarActor(), -1);
	}

	Client_OnUpdateAbilityStatus(AbilityTag, Status, Spec->Level);
	MarkAbilitySpecDirty(*Spec);
}

void UP16AbilitySystemComponent::Client_OnUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
{
	OnAbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
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
