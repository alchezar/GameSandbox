// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16ProjectileSpell.h"
#include "P16Fireblast.generated.h"

class AP16BlastBall;

UCLASS()
class PROJECT16_API UP16Fireblast : public UP16ProjectileSpell
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual FString GetDescription(const int32 CurrentLevel) override;
	virtual FString GetDescriptionNextLevel(const int32 CurrentLevel) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	TArray<AP16BlastBall*> SpawnBlastBalls(const FGameplayTag& SocketTag) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	int32 NumBalls = 12;
};
