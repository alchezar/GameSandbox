// Copyright (C) 2023, IKinder

#include "Player/AnimNotify/P12AnimNotify_EnableRagdoll.h"

void UP12AnimNotify_EnableRagdoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	OnEnableRagdoll.Broadcast(MeshComp);
}
