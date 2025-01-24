// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P12Interactable.generated.h"

class AP12BaseCharacter;

// This class does not need to be modified.
UINTERFACE()
class UP12Interactable : public UInterface
{
	GENERATED_BODY()
};

class PROJECT12_API IP12Interactable
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FP12OnInteraction)

	virtual void Interact(AP12BaseCharacter* Char)	 PURE_VIRTUAL(IP12Interactable::Interact, /* nothing to return */);
	virtual FName GetActionEventName() const		 PURE_VIRTUAL(ThisClass::GetActionEventName, return FName(NAME_None); );

	virtual bool HasOnInteractionCallback() const										 PURE_VIRTUAL(ThisClass::HasOnInteractionCallback, return false; );
	virtual FDelegateHandle AddOnInteractionFunction(UObject* Object, const FName& Name) PURE_VIRTUAL(ThisClass::AddOnInteractionDelegate, return FDelegateHandle(); );
	virtual void RemoveOnInteractionDelegate(FDelegateHandle Delegate)					 PURE_VIRTUAL(ThisClass::RemoveOnInteractionDelegate, );
};
