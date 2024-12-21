// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARGameplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UARGameplayInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT06_API IARGameplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kinder | Interaction")
	void Interact(APawn* InstigatorPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Kinder | Interaction")
	void OnActorLoaded();
};
