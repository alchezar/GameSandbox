// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12StreamingSubsystemVolume.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "P12StreamingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GAMESANDBOX_API UP12StreamingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void OnVolumeBeginOverlap(AP12StreamingSubsystemVolume* AP12StreamingSubsystemVolume);
	void OnVolumeOverlapEnd(AP12StreamingSubsystemVolume* AP12StreamingSubsystemVolume);
	bool GetCanUseSubsystem();
};
