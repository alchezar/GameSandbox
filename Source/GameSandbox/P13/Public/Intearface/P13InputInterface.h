// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P13InputInterface.generated.h"

struct FInputActionValue;
// This class does not need to be modified.
UINTERFACE()
class UP13InputInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMESANDBOX_API IP13InputInterface
{
	GENERATED_BODY()

public:
	virtual void MoveInput(const FVector2D MoveVector) = 0;
	virtual void SprintInput(const bool bStart) = 0;
	virtual void AimInput() = 0;
	virtual void ZoomInput(const float Axis) = 0;
	virtual void RotateTowardMovement(const FVector& Direction) = 0;
	virtual void FireInput(const bool bStart) = 0;
	virtual void ReloadInput() = 0;
	virtual void SwitchWeaponInput(bool bNext) = 0;
	virtual void DropInput() = 0;
};
