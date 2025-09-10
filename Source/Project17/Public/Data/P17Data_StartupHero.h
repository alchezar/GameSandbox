// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P17Data_StartupBase.h"
#include "Util/P17Types.h"
#include "P17Data_StartupHero.generated.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Startup", meta = (TitleProperty = "InputTag"))
	TArray<FP17HeroAbilitySet> HeroStartupAbilitySets = {};
};
