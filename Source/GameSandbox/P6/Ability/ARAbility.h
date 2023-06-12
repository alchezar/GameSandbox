// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ARAbility.generated.h"

class UWorld;

UCLASS(Blueprintable)
class GAMESANDBOX_API UARAbility : public UObject
{
	GENERATED_BODY()

public:
	UWorld* GetWorld() const override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	void StartAction(AActor* Instigator);
	UFUNCTION(BlueprintNativeEvent, Category = "C++ | Action")
	void StopAction(AActor* Instigator);

public:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Action")
	FName ActionName;
};
