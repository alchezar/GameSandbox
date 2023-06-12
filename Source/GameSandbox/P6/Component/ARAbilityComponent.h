// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ARAbilityComponent.generated.h"

class UARAbility;
class UARAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UARAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UARAbilityComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	void AddAbility(TSubclassOf<UARAbility> ActionClass);
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	bool StartAbilityByName(AActor* Instigator, FName ActionName);
	UFUNCTION(BlueprintCallable, Category = "C++ | Action")
	bool StopAbilityByName(AActor* Instigator, FName ActionName);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<UARAbility*> Abilities;
};
