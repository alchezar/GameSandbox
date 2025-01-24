// Copyright © 2024, IKinder

#include "Library/P13Types.h"

#include "Effect/P13StateEffect.h"

void UP13Types::AddEffectBySurfaceType(AActor* Victim, const TSubclassOf<UP13StateEffect> StateEffectClass, const EPhysicalSurface SurfaceType, AController* EffectCauser)
{
	if (!Victim || !StateEffectClass || SurfaceType == SurfaceType_Default)
	{
		return;
	}
	const UP13StateEffect* DefaultStateEffect = Cast<UP13StateEffect>(StateEffectClass.GetDefaultObject());
	if (!DefaultStateEffect)
	{
		return;
	}
	for (TEnumAsByte<EPhysicalSurface> PossibleSurface : DefaultStateEffect->GetPossibleInteractSurfaces())
	{
		if (PossibleSurface != SurfaceType)
		{
			continue;
		}
		UP13StateEffect* NewStateEffect = NewObject<UP13StateEffect>(Victim, StateEffectClass);
		check(NewStateEffect)
		NewStateEffect->InitObject(Victim, EffectCauser);
	}
}
