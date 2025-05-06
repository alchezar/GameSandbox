// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "P16GameInstance.generated.h"

class UP16MVVMLoadSlot;

UCLASS()
class PROJECT16_API UP16GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void UpdateGameParams(FString InSlotName, const int32 InSlotIndex, const FName InStartTag);
	void UpdateGameParams(const UP16MVVMLoadSlot* InLoadSlot);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY()
	FName PlayerStartTag = "Start";
	UPROPERTY()
	FString LoadSlotName = "Default";
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
