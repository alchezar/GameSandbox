// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimNotify/P12AnimNotify_ReloadOneAmmo.h"

void UP12AnimNotify_ReloadOneAmmo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnOneReload.Broadcast(MeshComp, NumberOfAmmo);
}
