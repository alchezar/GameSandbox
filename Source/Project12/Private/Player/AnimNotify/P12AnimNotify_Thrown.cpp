// Copyright (C) 2023, IKinder

#include "Player/AnimNotify/P12AnimNotify_Thrown.h"

void UP12AnimNotify_Thrown::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OnThrowableTaken.Broadcast(MeshComp);
}

void UP12AnimNotify_Thrown::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	OnThrowableLaunched.Broadcast(MeshComp);
}
