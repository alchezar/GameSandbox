// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "P17ControllerHero.generated.h"

UCLASS()
class PROJECT17_API AP17ControllerHero : public APlayerController
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP17ControllerHero();

	/// ------------------------------------------------------------------------
	/// @name Interface
	/// ------------------------------------------------------------------------
public:
	/// @par IGenericTeamAgentInterface ----------------------------------------
	virtual FGenericTeamId GetGenericTeamId() const override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	FGenericTeamId HeroTeamId = 0;
};
