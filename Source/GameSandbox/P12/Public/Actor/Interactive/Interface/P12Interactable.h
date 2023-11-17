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

class GAMESANDBOX_API IP12Interactable
{
	GENERATED_BODY()

public:
	virtual void Interact(AP12BaseCharacter* Char) = 0; /*PURE_VIRTUAL(IP12Interactable::Interact, /* nothing to return #1#);*/
	virtual FName GetActionEventName() const = 0; /*PURE_VIRTUAL(ThisClass::GetActionEventName, return FName(NAME_None); );*/
};
