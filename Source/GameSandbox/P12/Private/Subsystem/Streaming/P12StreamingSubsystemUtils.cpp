// Copyright (C) 2023, IKinder

#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystemUtils.h"

#include "GameFramework/Character.h"
#include "P12/Public/Subsystem/Streaming/P12StreamingSubsystemManager.h"

void UP12StreamingSubsystemUtils::CheckCharacterOverlapStreamingSubsystemVolume(ACharacter* Character)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemUtils::CheckCharacterOverlapStreamingSubsystemVolume(): Character: %s"), *GetNameSafe(Character));

	if (!Character)
	{
		return;
	}
	TSet<AActor*> OverlappingActors;
	Character->GetOverlappingActors(OverlappingActors, AP12StreamingSubsystemVolume::StaticClass());
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (!OverlappingActor || !OverlappingActor->HasActorBegunPlay())
		{
			continue;
		}
		StaticCast<AP12StreamingSubsystemVolume*>(OverlappingActor)->HandleCharacterOverlapBegin(Character);
	}
}

void UP12StreamingSubsystemUtils::CheckStreamingSubsystemVolumeOverlapCharacter(AP12StreamingSubsystemVolume* StreamingSubsystemVolume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemUtils::CheckStreamingSubsystemVolumeOverlapCharacter(): StreamingSubsystemVolume: %s"), *GetNameSafe(StreamingSubsystemVolume));

	if (!StreamingSubsystemVolume)
	{
		return;
	}
	TSet<AActor*> OverlappingActors;
	StreamingSubsystemVolume->GetOverlappingActors(OverlappingActors, ACharacter::StaticClass());
	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (!OverlappingActor || !OverlappingActor->HasActorBegunPlay())
		{
			continue;
		}
		StreamingSubsystemVolume->HandleCharacterOverlapBegin(StaticCast<ACharacter*>(OverlappingActor));
	}
}
