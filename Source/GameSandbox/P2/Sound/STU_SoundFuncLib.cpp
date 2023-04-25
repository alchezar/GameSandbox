// Copyright (C) 2023, IKinder

#include "STU_SoundFuncLib.h"
#include "Sound/SoundClass.h"

void USTU_SoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (!SoundClass) return;
		SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.f, 1.f);
}

void USTU_SoundFuncLib::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if (!SoundClass) return;

	const float NextVolume = SoundClass->Properties.Volume > 0.f ? 0.f : 1.f;
	SetSoundClassVolume(SoundClass, NextVolume);
}
