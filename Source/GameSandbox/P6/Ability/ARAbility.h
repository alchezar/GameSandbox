// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "ARAbility.generated.h"

class UARAbilityComponent;
class UWorld;

UCLASS(Blueprintable)
class GAMESANDBOX_API UARAbility : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;

	bool GetIsRunning() const;
	
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	void StartAbility(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	void StopAbility(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	bool CanStart(AActor* Instigator);

protected:
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	UARAbilityComponent* GetAbilityComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Action")
	FName AbilityName;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTagContainer GrantsTags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Tag")
	FGameplayTagContainer BlockedTags;

private:
	bool bRunning;
};
