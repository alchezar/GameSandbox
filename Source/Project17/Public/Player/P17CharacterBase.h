// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "P17CharacterBase.generated.h"

UCLASS()
class PROJECT17_API AP17CharacterBase : public ACharacter
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17CharacterBase();

protected:
	virtual void BeginPlay() override;
};
