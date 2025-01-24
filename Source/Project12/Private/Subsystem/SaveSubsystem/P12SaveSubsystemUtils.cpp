// Copyright (C) 2023, IKinder

#include "Subsystem/SaveSubsystem/P12SaveSubsystemUtils.h"

#include "Subsystem/SaveSubsystem/P12SaveSubsystemInterface.h"
#include "Subsystem/SaveSubsystem/P12SaveSubsystemTypes.h"

void UP12SaveSubsystemUtils::BroadcastOnLevelDeserialized(ULevel* Level)
{
	UE_LOG(LogP12SaveSubsystem, Log, TEXT("UP12SaveSubsystemUtils::BroadcastOnLevelDeserialized(): %s"), *GetNameSafe(Level));
	if (!Level)
	{
		return;
	}
	for (AActor* Actor : Level->Actors)
	{
		if (Actor && Actor->Implements<UP12SaveSubsystemInterface>())
		{
			IP12SaveSubsystemInterface::Execute_OnLevelDeserialized(Actor);
		}
	}
}
