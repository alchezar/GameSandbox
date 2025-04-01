// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interface/P16CombatInterface.h"
#include "Interface/P16PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/P16PlayerController.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

#define DEFINE_ONREP_GAMEPLAYATTRIBUTE(ClassName, PropertyName)                             \
void ClassName::OnRep_##PropertyName(const FGameplayAttributeData& Old##PropertyName) const \
{                                                                                           \
	GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, PropertyName, Old##PropertyName)                 \
}

UP16AttributeSet::UP16AttributeSet() {}

void UP16AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// clang-format off
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength,     COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resilience,   COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vigor,        COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Armor,                 COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorPenetration,      COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BlockChance,           COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance,     COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage,     COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegeneration,    COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegeneration,      COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth,             COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana,               COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArcaneResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, FireResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, LightningResistance, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, PhysicalResistance, COND_None, REPNOTIFY_Always)

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana,   COND_None, REPNOTIFY_Always)
	// clang-format on
}

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Strength)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Intelligence)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Resilience)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Vigor)

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Armor)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, ArmorPenetration)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, BlockChance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, CriticalHitChance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, CriticalHitDamage)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, CriticalHitResistance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, HealthRegeneration)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, ManaRegeneration)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, MaxHealth)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, MaxMana)

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, ArcaneResistance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, FireResistance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, LightningResistance)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, PhysicalResistance)

DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Health)
DEFINE_ONREP_GAMEPLAYATTRIBUTE(UP16AttributeSet, Mana)

void UP16AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp values before the changes are actually happens is a bad idea.
	// if (Attribute == GetHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	// }
	// else if (Attribute == GetManaAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	// }
}

void UP16AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FP16EffectProperties Properties = GetEffectProperties(Data);

	// Correct place to clamp attribute values.
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	// Using the incoming damage meta attribute to calculate health changes.
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Properties);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Properties);
	}
}

void UP16AttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, const float OldValue, const float NewValue)
{
	if (bTopOfHealth && Attribute == GetMaxHealthAttribute())
	{
		SetHealth(GetMaxHealth());
		bTopOfHealth = false;
	}
	if (bTopOfMana && Attribute == GetMaxManaAttribute())
	{
		SetMana(GetMaxMana());
		bTopOfMana = false;
	}
}

FP16EffectProperties UP16AttributeSet::GetEffectProperties(const FGameplayEffectModCallbackData& InData) const
{
	FP16EffectProperties Result;
	Result.EffectContext = InData.EffectSpec.GetContext();

	// Source fields.
	Result.SourceAbilitySystem = Result.EffectContext.GetOriginalInstigatorAbilitySystemComponent();
	EARLY_RETURN_VALUE_IF(!Result.SourceAbilitySystem || !Result.SourceAbilitySystem->AbilityActorInfo, Result)

	Result.SourceAvatarActor = Result.SourceAbilitySystem->AbilityActorInfo->AvatarActor.Get();
	EARLY_RETURN_VALUE_IF(!Result.SourceAvatarActor, Result)

	Result.SourceController = Result.SourceAbilitySystem->AbilityActorInfo->PlayerController.Get();
	if (!Result.SourceController && Result.SourceAvatarActor; const APawn* SourcePawn = Cast<APawn>(Result.SourceAvatarActor))
	{
		Result.SourceController = SourcePawn->GetController();
	}

	EARLY_RETURN_VALUE_IF(!Result.SourceController, Result)

	Result.SourceCharacter = Cast<ACharacter>(Result.SourceController->GetPawn());

	// Target fields.
	EARLY_RETURN_VALUE_IF(!InData.Target.AbilityActorInfo || !InData.Target.AbilityActorInfo->AvatarActor.Get(), Result)

	Result.TargetAvatarActor   = InData.Target.GetAvatarActor();
	Result.TargetController    = InData.Target.AbilityActorInfo->PlayerController.Get();
	Result.TargetCharacter     = Cast<ACharacter>(Result.TargetAvatarActor);
	Result.TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Result.TargetAvatarActor.Get());

	return Result;
}

void UP16AttributeSet::HandleIncomingDamage(const FP16EffectProperties& Properties)
{
	// Remember and reset incoming damage.
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	EARLY_RETURN_IF(LocalIncomingDamage <= 0.f)

	// Change health attribute.
	const float NewHealth = GetHealth() - LocalIncomingDamage;
	SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
	const bool bFatal = NewHealth <= 0.f;

	// Show damage numbers.
	ShowFloatingText(Properties, LocalIncomingDamage);

	// Activate the ability that plays the hit react montage.
	if (!bFatal)
	{
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FGSGameplayTagsSingleton::Get().P16Tags.Effect.HitReactTag);
		Properties.TargetAbilitySystem->TryActivateAbilitiesByTag(TagContainer);
		return;
	}

	// Death reaction.
	const TScriptInterface<IP16CombatInterface> CombatInterface = Properties.TargetAvatarActor.Get();
	EARLY_RETURN_IF(!CombatInterface)
	CombatInterface->Die();
	SendRewardXPEvent(Properties);
}

void UP16AttributeSet::HandleIncomingXP(const FP16EffectProperties& Properties)
{
	// Source character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP.
	ACharacter* Owner = Properties.SourceCharacter;
	EARLY_RETURN_IF(!Owner
		|| !Properties.SourceCharacter->Implements<UP16PlayerInterface>()
		|| !Properties.TargetCharacter->Implements<UP16CombatInterface>())

	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	// Try to level up.
	const int32 CurrentLevel = IP16CombatInterface::Execute_GetPlayerLevel(Owner);
	const int32 CurrentXP    = IP16PlayerInterface::Execute_GetXP(Owner);
	const int32 NewLevel     = IP16PlayerInterface::Execute_GetLevelFor(Owner, CurrentXP + LocalIncomingXP);
	const int32 LevelUpsNum  = NewLevel - CurrentLevel;

	for (int32 Index = 0; Index < LevelUpsNum; ++Index)
	{
		const int32 AttributePointsReward = IP16PlayerInterface::Execute_GetAttributePointsReward(Owner, CurrentLevel);
		const int32 SpellPointsReward     = IP16PlayerInterface::Execute_GetSpellPointsReward(Owner, CurrentLevel);

		// Level up the player and add rewards points
		IP16PlayerInterface::Execute_AddAttributePoints(Owner, AttributePointsReward);
		IP16PlayerInterface::Execute_AddSpellPoints(Owner, SpellPointsReward);

		// Fill Health and Mana in PostAttributeChange(...).
		bTopOfHealth = true;
		bTopOfMana   = true;
	}
	if (NewLevel > CurrentLevel)
	{
		IP16PlayerInterface::Execute_AddToLevel(Owner, NewLevel - CurrentLevel);
		IP16PlayerInterface::Execute_LevelUp(Owner);
	}

	// Add the XP to the player.
	IP16PlayerInterface::Execute_AddToXP(Owner, LocalIncomingXP);
}

void UP16AttributeSet::ShowFloatingText(const FP16EffectProperties& Properties, const float InDamage) const
{
	const ACharacter* Source = Properties.SourceCharacter;
	ACharacter*       Target = Properties.TargetCharacter;

	const bool bBlockingHit = UP16AbilitySystemLibrary::GetIsBlockedHit(Properties.EffectContext);
	const bool bCriticalHit = UP16AbilitySystemLibrary::GetIsCriticalHit(Properties.EffectContext);

	EARLY_RETURN_IF(!Source || !Target || Source == Target)
	if (AP16PlayerController* SourceController = Source->GetController<AP16PlayerController>())
	{
		SourceController->Client_ShowDamageNumber(InDamage, Target, bBlockingHit, bCriticalHit);
	}
	if (AP16PlayerController* TargetController = Target->GetController<AP16PlayerController>())
	{
		TargetController->Client_ShowDamageNumber(InDamage, Target, bBlockingHit, bCriticalHit);
	}
}

void UP16AttributeSet::SendRewardXPEvent(const FP16EffectProperties& Properties) const
{
	ACharacter* TargetCharacter = Properties.TargetCharacter;
	EARLY_RETURN_IF(!TargetCharacter || !TargetCharacter->Implements<UP16CombatInterface>())

	const EP16CharacterClass TargetClass = IP16CombatInterface::Execute_GetCharacterClass(TargetCharacter);
	const int32              TargetLevel = IP16CombatInterface::Execute_GetPlayerLevel(TargetCharacter);

	const FGameplayTag Tag     = FGSGameplayTagsSingleton::Get().P16Tags.Attribute.Meta.IncomingXPTag;
	FGameplayEventData Payload = {};
	Payload.EventTag           = Tag;
	Payload.EventMagnitude     = UP16AbilitySystemLibrary::GetXPRewardFor(Properties.TargetCharacter, TargetClass, TargetLevel);

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Properties.SourceCharacter, Tag, MoveTemp(Payload));
}
