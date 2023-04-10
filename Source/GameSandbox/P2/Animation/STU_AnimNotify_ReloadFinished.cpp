// Copyright (C) 2023, IKinder

#include "STU_AnimNotify_ReloadFinished.h"

void USTU_AnimNotify_ReloadFinished::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	OnReloadedNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation, EventReference);
}