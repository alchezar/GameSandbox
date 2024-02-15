// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13/Public/Effect/P13StateEffect.h"
#include "UObject/Interface.h"
#include "P13StateEffectInterface.generated.h"

UINTERFACE()
class UP13StateEffectInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMESANDBOX_API IP13StateEffectInterface
{
	GENERATED_BODY()

public:
	virtual EPhysicalSurface GetSurfaceType() = 0;
	virtual bool GetCanApplyStateEffect(const TSubclassOf<UP13StateEffect> StateEffectClass = nullptr) const = 0;
	virtual void AddActiveStateEffect(UP13StateEffect* StateEffect) = 0;
	virtual void RemoveInactiveStateEffect(UP13StateEffect* InactiveStateEffect) = 0;
};
