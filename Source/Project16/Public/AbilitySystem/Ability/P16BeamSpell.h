// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16DamageGameplayAbility.h"
#include "P16BeamSpell.generated.h"

UCLASS()
class PROJECT16_API UP16BeamSpell : public UP16DamageGameplayAbility
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
	void StoreMouseDataInfo(const FHitResult& HitResult);
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	FVector MouseHitLocation = {};
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	TObjectPtr<AActor> MouseHitActor = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	TObjectPtr<APlayerController> OwnerController = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;
};
