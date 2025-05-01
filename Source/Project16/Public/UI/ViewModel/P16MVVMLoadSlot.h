// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "P16MVVMLoadSlot.generated.h"

UCLASS()
class PROJECT16_API UP16MVVMLoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	FString GetLoadSlotName() const { return LoadSlotName; };
	void    SetLoadSlotName(const FString& InLoadSlotName);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Getter, Setter, Category = "C++")
	FString LoadSlotName;
};
