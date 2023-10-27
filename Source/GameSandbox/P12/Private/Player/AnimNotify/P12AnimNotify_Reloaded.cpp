// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimNotify/P12AnimNotify_Reloaded.h"

void UP12AnimNotify_Reloaded::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnFullReloaded.Broadcast(MeshComp);
}
