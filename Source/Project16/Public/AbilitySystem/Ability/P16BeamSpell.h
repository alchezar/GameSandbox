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
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> StoreAdditionalTargets() const;

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
	UPROPERTY(BlueprintReadWrite, Category = "C++")
	int32 MaxNumTargets = 5;
};
