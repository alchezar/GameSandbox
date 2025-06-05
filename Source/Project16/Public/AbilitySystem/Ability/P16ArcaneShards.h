// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16DamageGameplayAbility.h"
#include "P16ArcaneShards.generated.h"

UCLASS()
class PROJECT16_API UP16ArcaneShards : public UP16DamageGameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual FString GetDescription(const int32 CurrentLevel) override;
	virtual FString GetDescriptionNextLevel(const int32 CurrentLevel) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	int32 MaxPointsNum = 12;
};
