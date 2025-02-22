// Copyright (C) 2023, IKinder

#include "AnimNotify/P9NotifyWhoosh.h"

void UP9NotifyWhoosh::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnWhoosh.Broadcast(MeshComp);
}
