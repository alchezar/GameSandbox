// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/Ability/P16DamageGameplayAbility.h"
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
		return Spec.Ability->GetAssetTags().GetGameplayTagArray().ContainsByPredicate(AnyOf);
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
	const FGameplayTag* FoundTag = InAbilitySpec
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
	const FGameplayTag* FoundTag = InAbilitySpec
		.GetDynamicSpecSourceTags()
		.GetGameplayTagArray()
		.FindByPredicate([](const FGameplayTag Tag) -> bool
		{
			return Tag.MatchesTag(FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status.Tag);
		});

	return FoundTag ? *FoundTag : FGameplayTag {};
}

FGameplayTag UP16AbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& InTag)
{
	const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(InTag);
	EARLY_RETURN_VALUE_IF(!Spec, FGameplayTag {})
	return GetStatusFromSpec(*Spec);
}

FGameplayTag UP16AbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& InTag)
{
	const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(InTag);
	EARLY_RETURN_VALUE_IF(!Spec, FGameplayTag{})
	return GetInputTagFromSpec(*Spec);
}

FP16AbilityDescription UP16AbilitySystemComponent::GetDescription(const FGameplayTag& AbilityTag)
{
	FP16AbilityDescription Result = {};

	// For deselected spell globe.
	EARLY_RETURN_VALUE_IF(AbilityTag.MatchesTagExact(FGSGameplayTagsSingleton::Get().P16Tags.Ability.NoneTag), Result)

	const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag);
	if (!Spec)
	{
		const UP16AbilityInfoDataAsset* AbilityInfo = UP16AbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
		check(AbilityInfo)
		Result = {UP16DamageGameplayAbility::GetDescriptionLocked(AbilityInfo->FindAbilityInfo(AbilityTag).LevelRequirement), {}};
		return Result;
	}

	UP16GameplayAbility* Ability = Cast<UP16GameplayAbility>(Spec->Ability);
	EARLY_RETURN_VALUE_IF(!Ability, Result)
	Result = {Ability->GetDescription(Spec->Level), Ability->GetDescriptionNextLevel(Spec->Level)};

	return Result;
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

void UP16AbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	FScopedAbilityListLock Lock {*this};
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!GetAbilityHasSlot(AbilitySpec, InputTag))
		AbilitySpecInputPressed(AbilitySpec);

		CONTINUE_IF(!AbilitySpec.IsActive())
		// More comprehensive approach to handle `InstancedPerExecution` policy,
		// because `GetPrimaryInstance()` is only valid on `InstancePerActor` policy.
		for (const UGameplayAbility* AbilityInstance : AbilitySpec.GetAbilityInstances())
		{
			InvokeReplicatedEvent(
				EAbilityGenericReplicatedEvent::InputPressed,
				AbilitySpec.Handle,
				AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
		}
	}
}

void UP16AbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	FScopedAbilityListLock Lock {*this};
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!GetAbilityHasSlot(AbilitySpec, InputTag))

		AbilitySpecInputPressed(AbilitySpec);
		CONTINUE_IF(AbilitySpec.IsActive())

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UP16AbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	EARLY_RETURN_IF(!InputTag.IsValid())

	FScopedAbilityListLock Lock {*this};
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		CONTINUE_IF(!GetAbilityHasSlot(AbilitySpec, InputTag)
			|| !AbilitySpec.IsActive())

		AbilitySpecInputReleased(AbilitySpec);
		// More comprehensive approach to handle `InstancedPerExecution` policy,
		// because `GetPrimaryInstance()` is only valid on `InstancePerActor` policy.
		for (const UGameplayAbility* AbilityInstance : AbilitySpec.GetAbilityInstances())
		{
			InvokeReplicatedEvent(
				EAbilityGenericReplicatedEvent::InputReleased,
				AbilitySpec.Handle,
				AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
		}
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

void UP16AbilitySystemComponent::UpdateStunStatus(const bool bStun)
{
	FGameplayTagContainer BlockedTagsContainer;
	const auto            BlockedTags = FGSGameplayTagsSingleton::Get().P16Tags.Player.Blocked;
	BlockedTagsContainer.AddTag(BlockedTags.InputPressedTag);
	BlockedTagsContainer.AddTag(BlockedTags.InputHeldTag);
	BlockedTagsContainer.AddTag(BlockedTags.InputReleasedTag);
	BlockedTagsContainer.AddTag(BlockedTags.CursorTraceTag);

	AddLooseGameplayTags(BlockedTagsContainer, bStun ? 1 : -1);
}

void UP16AbilitySystemComponent::Server_EquipAbility_Implementation(const FGameplayTag& InAbilityTag, const FGameplayTag& SlotInputTag)
{
	FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(InAbilityTag);
	EARLY_RETURN_IF(!Spec)

	const FGameplayTag& PrevSlotInput = GetInputTagFromSpec(*Spec);
	const FGameplayTag& Status        = GetStatusFromSpec(*Spec);

	const auto StatusTags   = FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status;
	const bool bStatusValid = Status.MatchesTagExact(StatusTags.EquippedTag) || Status.MatchesTagExact(StatusTags.UnlockedTag);
	EARLY_RETURN_IF(!bStatusValid)

	// There is an ability in this slot already. Deactivate and clear that slot.
	if (!GetSlotIsEmpty(SlotInputTag))
	{
		if (FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(SlotInputTag))
		{
			const FGameplayTag AbilityTag = GetAbilityTagFromSpec(*SpecWithSlot);
			// If that ability is the same as this ability, we can return early.
			if (InAbilityTag.MatchesTagExact(AbilityTag))
			{
				Client_EquipAbility(InAbilityTag, SlotInputTag, PrevSlotInput, Status);
				return;
			}
			if (GetIsPassiveAbility(*SpecWithSlot))
			{
				OnDeactivatePassiveAbility.Broadcast(AbilityTag);
			}
			ClearSlot(SpecWithSlot);
		}
	}

	// Ability doesn't yet have a slot, it's not active.
	if (!GetAbilityHasAnySlot(*Spec) && GetIsPassiveAbility(*Spec))
	{
		TryActivateAbility(Spec->Handle);
	}

	AssignSlotToAbility(*Spec, SlotInputTag);
	MarkAbilitySpecDirty(*Spec);
	Client_EquipAbility(InAbilityTag, SlotInputTag, PrevSlotInput, Status);
}

void UP16AbilitySystemComponent::Client_EquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& SlotInputTag, const FGameplayTag& PreviousSlotInputTag, const FGameplayTag& StatusTag)
{
	OnAbilityEquipped.Broadcast(AbilityTag, SlotInputTag, PreviousSlotInputTag, StatusTag);
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

void UP16AbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* InSpec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*InSpec);
	InSpec->GetDynamicSpecSourceTags().RemoveTag(Slot);

	// Remove equipped status for the ability in cleared slot. Maybe this is too much,
	// but it gives me confidence that all clears and assigns correctly.
	const FGameplayTag& Status     = GetStatusFromSpec(*InSpec);
	const auto          StatusTags = FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status;
	InSpec->GetDynamicSpecSourceTags().RemoveTag(StatusTags.EquippedTag);
	InSpec->GetDynamicSpecSourceTags().AddTag(StatusTags.UnlockedTag);
}

void UP16AbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& InSlotInputTag)
{
	// Don't use predicates here, because `FilterByPredicate` returns `TArray<ElementType>` without `&`!
	FScopedAbilityListLock Lock {*this};
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (GetAbilityHasSlot(AbilitySpec, InSlotInputTag))
		{
			ClearSlot(&AbilitySpec);
		}
	}
}

bool UP16AbilitySystemComponent::GetSlotIsEmpty(const FGameplayTag& InSlotInputTag)
{
	FScopedAbilityListLock Lock {*this};

	auto AnyOf = [this, InSlotInputTag](const FGameplayAbilitySpec& AbilitySpec) -> bool
	{
		return GetAbilityHasSlot(AbilitySpec, InSlotInputTag);
	};
	return !GetActivatableAbilities().ContainsByPredicate(AnyOf);
}

bool UP16AbilitySystemComponent::GetAbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.GetDynamicSpecSourceTags().HasTagExact(Slot);
}

bool UP16AbilitySystemComponent::GetAbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGSGameplayTagsSingleton::Get().P16Tags.Input.Tag);
}

FGameplayAbilitySpec* UP16AbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& InSlotInputTag)
{
	FScopedAbilityListLock Lock {*this};

	auto AnyOf = [InSlotInputTag](const FGameplayTag& Tag) -> bool
	{
		return InSlotInputTag.MatchesTag(Tag);
	};
	auto FindIf = [AnyOf](const FGameplayAbilitySpec& Spec) -> bool
	{
		return Spec.GetDynamicSpecSourceTags().GetGameplayTagArray().ContainsByPredicate(AnyOf);
	};

	return GetActivatableAbilities().FindByPredicate(FindIf);
}

bool UP16AbilitySystemComponent::GetIsPassiveAbility(const FGameplayAbilitySpec& Spec) const
{
	const UP16AbilityInfoDataAsset* AbilityInfoData = UP16AbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	EARLY_RETURN_VALUE_IF(!AbilityInfoData, false)

	const FP16AbilityInfo AbilityInfo = AbilityInfoData->FindAbilityInfo(GetAbilityTagFromSpec(Spec));
	const FGameplayTag    AbilityType = AbilityInfo.TypeTag;

	return AbilityType.MatchesTagExact(FGSGameplayTagsSingleton::Get().P16Tags.Ability.Type.PassiveTag);
}

void UP16AbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& SlotInputTag)
{
	// Clear this ability's slot, just in case it's a different slot.
	ClearSlot(&Spec);
	// Now, assign this ability to slot.
	Spec.GetDynamicSpecSourceTags().AddTag(SlotInputTag);
	// Add equipped status for this ability.
	const auto StatusTags = FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status;
	Spec.GetDynamicSpecSourceTags().RemoveTag(StatusTags.UnlockedTag);
	Spec.GetDynamicSpecSourceTags().AddTag(StatusTags.EquippedTag);
}
