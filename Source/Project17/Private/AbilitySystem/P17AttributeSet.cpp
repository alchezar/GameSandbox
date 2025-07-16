// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P17AttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Project17.h"
#include "Component/UI/P17UIHeroComponent.h"
#include "Component/UI/P17UIPawnComponent.h"
#include "Interface/P17UIInterface.h"
#include "Util/P17FunctionLibrary.h"
#include "Util/P17GameplayTags.h"

UP17AttributeSet::UP17AttributeSet()
{}

void UP17AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthCurrentAttribute())
	{
		const float NewHealthCurrent = FMath::Clamp(GetHealthCurrent(), 0.f, GetHealthMax());
		SetHealthCurrent(NewHealthCurrent);
		NotifyHealthChange(Data);
	}
	if (Data.EvaluatedData.Attribute == GetRageCurrentAttribute())
	{
		const float NewRageCurrent = FMath::Clamp(GetRageCurrent(), 0.f, GetRageMax());
		SetRageCurrent(NewRageCurrent);
		UpdateRageStatusTags(Data);
		NotifyRageChange(Data);
	}
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetHealthCurrent();
		const float Damage = GetDamageTaken();
		const float NewHealth = FMath::Clamp(OldHealth - Damage, 0.f, GetHealthMax());
		SetHealthCurrent(NewHealth);
		NotifyHealthChange(Data);

		if (FMath::IsNearlyZero(GetHealthCurrent()))
		{
			UP17FunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), P17::Tags::Shared_Status_Dead);
		}
	}
}

TWeakInterfacePtr<IP17UIInterface> UP17AttributeSet::GetCachedUIInterface(AActor* InActor)
{
	if (!CachedUIInterface.IsValid())
	{
		CachedUIInterface = TWeakInterfacePtr<IP17UIInterface> {InActor};
	}
	return CachedUIInterface.IsValid() ? CachedUIInterface : nullptr;
}

void UP17AttributeSet::NotifyRageChange(const FGameplayEffectModCallbackData& Data)
{
	const auto UIInterface = GetCachedUIInterface(Data.Target.GetAvatarActor());
	WARN_RETURN_IF(!UIInterface.IsValid(),)
	const UP17UIHeroComponent* HeroUIComponent = UIInterface->GetHeroUIComponent();
	WARN_RETURN_IF(!HeroUIComponent,)

	const float RagePercentage = GetRageCurrent() / GetRageMax();
	HeroUIComponent->OnRageChanged.Broadcast(RagePercentage);
}

void UP17AttributeSet::NotifyHealthChange(const FGameplayEffectModCallbackData& Data)
{
	const auto UIInterface = GetCachedUIInterface(Data.Target.GetAvatarActor());
	WARN_RETURN_IF(!UIInterface.IsValid(),)
	const UP17UIPawnComponent* PawnUIComponent = UIInterface->GetPawnUIComponent();
	WARN_RETURN_IF(!PawnUIComponent,)

	const float HealthPercentage = GetHealthCurrent() / GetHealthMax();
	PawnUIComponent->OnHealthChanged.Broadcast(HealthPercentage);
}

void UP17AttributeSet::UpdateRageStatusTags(const FGameplayEffectModCallbackData& Data) const
{
	AActor* Avatar = Data.Target.GetAvatarActor();
	if (FMath::IsNearlyEqual(GetRageCurrent(), GetRageMax(), 0.1f))
	{
		UP17FunctionLibrary::AddGameplayTagToActorIfNone(Avatar, P17::Tags::Player_Status_Rage_Full);
	}
	else if (FMath::IsNearlyZero(GetRageCurrent(), 0.1f))
	{
		UP17FunctionLibrary::AddGameplayTagToActorIfNone(Avatar, P17::Tags::Player_Status_Rage_None);
	}
	else
	{
		UP17FunctionLibrary::RemoveGameplayTagFromActorIfFound(Avatar, P17::Tags::Player_Status_Rage_Full);
		UP17FunctionLibrary::RemoveGameplayTagFromActorIfFound(Avatar, P17::Tags::Player_Status_Rage_None);
	}
}
