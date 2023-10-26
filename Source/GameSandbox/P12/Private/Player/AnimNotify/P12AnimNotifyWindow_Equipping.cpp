// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimNotify/P12AnimNotifyWindow_Equipping.h"

void UP12AnimNotifyWindow_Equipping::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OnEquippingStarted.Broadcast(MeshComp);
}

void UP12AnimNotifyWindow_Equipping::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	OnEquippingFinished.Broadcast(MeshComp);
}
