// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "P17Data_StartupBase.h"
#include "P17Data_StartupHero.generated.h"

USTRUCT(BlueprintType)
struct FP17HeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag Tag = {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UP17GameplayAbility> Ability = {};

public:
	bool IsValid() const;
};

UCLASS()
class PROJECT17_API UP17Data_StartupHero : public UP17Data_StartupBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual void GiveToAbilitySystemComponent(UP17AbilitySystemComponent* InASC, const int32 InLevel = 1) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Startup", meta = (TitleProperty = "Tag"))
	TArray<FP17HeroAbilitySet> HeroStartupAbilitySets = {};
};
