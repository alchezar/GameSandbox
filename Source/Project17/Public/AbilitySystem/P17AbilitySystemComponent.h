// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Util/P17Types.h"

#include "P17AbilitySystemComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT17_API UP17AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UP17AbilitySystemComponent();

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, meta = (ApplyLevel = "1"))
	void GrantHeroWeaponAbilities(const TArray<FP17HeroAbilitySet>& InWeaponAbilities, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles, const int32 InLevel = 1);

	// clang-format off
	UFUNCTION(BlueprintCallable)
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);
	// clang-format on
};
