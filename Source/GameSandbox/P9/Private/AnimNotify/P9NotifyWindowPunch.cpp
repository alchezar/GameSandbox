// Copyright (C) 2023, IKinder

#include "P9/Public/AnimNotify/P9NotifyWindowPunch.h"

UP9NotifyWindowPunch::UP9NotifyWindowPunch()
{
	NotifyName = "Punch";
}

void UP9NotifyWindowPunch::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OnPunchStart.Broadcast(MeshComp);
}

void UP9NotifyWindowPunch::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	OnPunchEnd.Broadcast(MeshComp);
}
