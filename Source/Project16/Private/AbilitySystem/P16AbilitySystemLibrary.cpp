// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "AbilitySystem/P16GameplayEffectContext.h"
#include "AbilitySystem/Ability/P16GameplayAbility.h"
#include "AbilitySystem/Data/P16CharacterClassInfoDataAsset.h"
#include "Game/P16GameMode.h"
#include "Interface/P16CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/P16PlayerState.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"
#include "UI/HUD/P16HUD.h"

UP16OverlayWidgetController* UP16AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	AP16HUD*                   HUD    = nullptr;
	FP16WidgetControllerParams Params = GetWidgetControllerParams(WorldContextObject, &HUD);
	EARLY_RETURN_VALUE_IF(!HUD, nullptr)

	return HUD->GetOverlayWidgetController(MoveTemp(Params));
}

UP16AttributeMenuWidgetController* UP16AbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	AP16HUD*                   HUD    = nullptr;
	FP16WidgetControllerParams Params = GetWidgetControllerParams(WorldContextObject, &HUD);
	EARLY_RETURN_VALUE_IF(!HUD, nullptr)

	return HUD->GetAttributeMenuWidgetController(MoveTemp(Params));
}

UP16SpellMenuWidgetController* UP16AbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	AP16HUD*                   HUD    = nullptr;
	FP16WidgetControllerParams Params = GetWidgetControllerParams(WorldContextObject, &HUD);
	EARLY_RETURN_VALUE_IF(!HUD, nullptr)

	return HUD->GetSpellMenuWidgetController(MoveTemp(Params));
}

void UP16AbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContextObject, const EP16CharacterClass CharacterClass, UAbilitySystemComponent* AbilitySystemComponent, const float Level)
{
	EARLY_RETURN_IF(!WorldContextObject)
	UP16CharacterClassInfoDataAsset* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	EARLY_RETURN_IF(!ClassInfo)

	const AActor*                       Avatar      = AbilitySystemComponent->GetAvatarActor();
	const FP16CharacterClassDefaultInfo DefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

	ApplyGameplayEffect(AbilitySystemComponent, DefaultInfo.PrimaryAttributes, Level, Avatar);
	ApplyGameplayEffect(AbilitySystemComponent, ClassInfo->SecondaryAttributes, Level, Avatar);
	ApplyGameplayEffect(AbilitySystemComponent, ClassInfo->VitalAttributes, Level, Avatar);
}

void UP16AbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, const EP16CharacterClass CharacterClass)
{
	// Add common abilities.
	EARLY_RETURN_IF(!AbilitySystemComponent)
	UP16CharacterClassInfoDataAsset* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	EARLY_RETURN_IF(!ClassInfo)
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : ClassInfo->CommonAbilities)
	{
		AbilitySystemComponent->GiveAbility({AbilityClass});
	}

	// Add character class specific abilities.
	EARLY_RETURN_IF(!ClassInfo->CharacterClassInfoMap.Contains(CharacterClass))

	AActor*     AvatarActor = AbilitySystemComponent->GetAvatarActor();
	const int32 PlayerLevel = AvatarActor && AvatarActor->Implements<UP16CombatInterface>() ? IP16CombatInterface::Execute_GetPlayerLevel(AvatarActor) : 1;

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : ClassInfo->GetClassDefaultInfo(CharacterClass).StartupAbilities)
	{
		AbilitySystemComponent->GiveAbility({AbilityClass, PlayerLevel});
	}
}

UP16CharacterClassInfoDataAsset* UP16AbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	EARLY_RETURN_VALUE_IF(!WorldContextObject, nullptr)
	const AP16GameMode* GameMode = Cast<AP16GameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	EARLY_RETURN_VALUE_IF(!GameMode, nullptr)

	return GameMode->GetCharacterClassInfo();
}

UP16AbilityInfoDataAsset* UP16AbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	EARLY_RETURN_VALUE_IF(!WorldContextObject, nullptr)
	const AP16GameMode* GameMode = Cast<AP16GameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	EARLY_RETURN_VALUE_IF(!GameMode, nullptr)

	return GameMode->GetAbilityIfo();
}

bool UP16AbilitySystemLibrary::GetIsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FP16GameplayEffectContext* Context = StaticCast<const FP16GameplayEffectContext*>(EffectContextHandle.Get());
	return Context ? Context->GetIsBlockedHit() : false;
}

bool UP16AbilitySystemLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FP16GameplayEffectContext* Context = StaticCast<const FP16GameplayEffectContext*>(EffectContextHandle.Get());
	return Context ? Context->GetIsCriticalHit() : false;
}

FP16DebuffSpec UP16AbilitySystemLibrary::GetDebuffSpec(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FP16GameplayEffectContext* Context = StaticCast<const FP16GameplayEffectContext*>(EffectContextHandle.Get());
	return Context ? Context->GetDebuffSpec() : FP16DebuffSpec {};
}

FGameplayTag UP16AbilitySystemLibrary::GetDebuffDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FP16GameplayEffectContext* Context = StaticCast<const FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_VALUE_IF(!Context || !Context->GetDebuffSpec().DamageType.IsValid(), FGameplayTag {})

	return *Context->GetDebuffSpec().DamageType;
}

FVector UP16AbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FP16GameplayEffectContext* Context = StaticCast<const FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_VALUE_IF(!Context, FVector::ZeroVector)

	return Context->GetDeadImpulse();
}

FVector UP16AbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FP16GameplayEffectContext* Context = StaticCast<const FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_VALUE_IF(!Context, FVector::ZeroVector)

	return Context->GetKnockbackForce();
}

void UP16AbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bNewBlocked)
{
	FP16GameplayEffectContext* Context = StaticCast<FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_IF(!Context)
	Context->SetIsBlockedHit(bNewBlocked);
}

void UP16AbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bNewCritical)
{
	FP16GameplayEffectContext* Context = StaticCast<FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_IF(!Context)
	Context->SetIsCriticalHit(bNewCritical);
}

void UP16AbilitySystemLibrary::SetDebuffSpec(FGameplayEffectContextHandle& EffectContextHandle, const bool bSuccessful, const FP16DebuffInfo& InDebuffInfo, const FGameplayTag& InDamageType)
{
	FP16GameplayEffectContext* Context = StaticCast<FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_IF(!Context)
	Context->SetDebuffSpec(bSuccessful, InDebuffInfo, InDamageType);
}

void UP16AbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	FP16GameplayEffectContext* Context = StaticCast<FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_IF(!Context)

	Context->SetDeathImpulse(InImpulse);
}

void UP16AbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	FP16GameplayEffectContext* Context = StaticCast<FP16GameplayEffectContext*>(EffectContextHandle.Get());
	EARLY_RETURN_IF(!Context)

	Context->SetKnockbackForce(InForce);
}

TArray<AActor*> UP16AbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, const TArray<AActor*> IgnoredActors, const float Radius, const FVector SphereOrigin)
{
	EARLY_RETURN_VALUE_IF(!WorldContextObject, TArray<AActor*>{})
	const UWorld* World = WorldContextObject->GetWorld();
	EARLY_RETURN_VALUE_IF(!World, TArray<AActor*>{})

	FCollisionQueryParams SphereParams {};
	SphereParams.AddIgnoredActors(IgnoredActors);

	TArray<FOverlapResult> OverlapResults {};
	World->OverlapMultiByObjectType(OverlapResults, SphereOrigin, FQuat::Identity, {{FCollisionObjectQueryParams::InitType::AllDynamicObjects}}, FCollisionShape::MakeSphere(Radius), SphereParams);

	TArray<AActor*> Result = {};
	for (const FOverlapResult& OverlapResult : OverlapResults)
	{
		AActor* Actor = OverlapResult.GetActor();
		CONTINUE_IF(!Actor || !Actor->Implements<UP16CombatInterface>() || IP16CombatInterface::Execute_GetIsDead(Actor))

		Result.AddUnique(Actor);
	}
	return Result;
}

bool UP16AbilitySystemLibrary::GetIsNotFriends(const AActor* First, const AActor* Second)
{
	EARLY_RETURN_VALUE_IF(!First || !Second, false)

	const bool bFirstPlayer  = First->ActorHasTag(P16::Tag::Player);
	const bool bSecondPlayer = Second->ActorHasTag(P16::Tag::Player);
	const bool bFriends      = bFirstPlayer == bSecondPlayer;
	const bool bEnemies      = bFirstPlayer != bSecondPlayer;

	return !bFriends || bEnemies;
}

int32 UP16AbilitySystemLibrary::GetXPRewardFor(const UObject* WorldContextObject, const EP16CharacterClass CharacterClass, const int32 Level)
{
	UP16CharacterClassInfoDataAsset* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	EARLY_RETURN_VALUE_IF(!ClassInfo, 0)
	const FP16CharacterClassDefaultInfo* FoundInfo = ClassInfo->CharacterClassInfoMap.Find(CharacterClass);
	EARLY_RETURN_VALUE_IF(!FoundInfo, 0)

	return static_cast<int32>(FoundInfo->XPReward.GetValueAtLevel(Level));
}

FGameplayEffectContextHandle UP16AbilitySystemLibrary::ApplyDamageEffect(const FP16DamageEffectParams& Params)
{
	EARLY_RETURN_VALUE_IF(!Params.TargetAbilitySystemComponent, {})

	FGameplayEffectContextHandle ContextHandle = Params.TargetAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(Params.SourceAbilitySystemComponent->GetAvatarActor());
	SetDeathImpulse(ContextHandle, Params.DeathImpulse.Velocity);
	SetKnockbackForce(ContextHandle, Params.Knockback.Velocity);

	const FGameplayEffectSpecHandle SpecHandle = Params.TargetAbilitySystemComponent->MakeOutgoingSpec(Params.DamageEffectClass, Params.AbilityLevel, ContextHandle);
	SpecHandle.Data->SetSetByCallerMagnitude(Params.DamageType, Params.BaseDamage);

	const auto DebuffInfoTags = FGSGameplayTagsSingleton::Get().P16Tags.Debuff.Info;
	SpecHandle.Data->SetSetByCallerMagnitude(DebuffInfoTags.ChanceTag, Params.Debuff.Chance);
	SpecHandle.Data->SetSetByCallerMagnitude(DebuffInfoTags.DamageTag, Params.Debuff.Damage);
	SpecHandle.Data->SetSetByCallerMagnitude(DebuffInfoTags.FrequencyTag, Params.Debuff.Frequency);
	SpecHandle.Data->SetSetByCallerMagnitude(DebuffInfoTags.DurationTag, Params.Debuff.Duration);

	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return ContextHandle;
}

FP16WidgetControllerParams UP16AbilitySystemLibrary::GetWidgetControllerParams(const UObject* WorldContextObject, AP16HUD** OutHUD)
{
	FP16WidgetControllerParams Result = {};
	EARLY_RETURN_VALUE_IF(!WorldContextObject, Result)

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	EARLY_RETURN_VALUE_IF(!PlayerController, Result)

	*OutHUD                      = PlayerController->GetHUD<AP16HUD>();
	AP16PlayerState* PlayerState = PlayerController->GetPlayerState<AP16PlayerState>();
	EARLY_RETURN_VALUE_IF(!*OutHUD || !PlayerState, Result)

	UAbilitySystemComponent* AbilitySystem = PlayerState->GetAbilitySystemComponent();
	UAttributeSet*           AttributeSet  = PlayerState->GetAttributeSet();

	Result = FP16WidgetControllerParams {PlayerController, PlayerState, AbilitySystem, AttributeSet};
	return Result;
}

void UP16AbilitySystemLibrary::ApplyGameplayEffect(UAbilitySystemComponent* AbilitySystemComponent, const TSubclassOf<UGameplayEffect>& GameplayEffect, const float Level, const AActor* SourceObject)
{
	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceObject);

	const FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, Level, ContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}
