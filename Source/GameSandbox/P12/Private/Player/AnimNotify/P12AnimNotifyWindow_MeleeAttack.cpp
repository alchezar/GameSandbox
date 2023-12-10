// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimNotify/P12AnimNotifyWindow_MeleeAttack.h"

void UP12AnimNotifyWindow_MeleeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OnMeleeAttackStart.Broadcast(MeshComp);
}

void UP12AnimNotifyWindow_MeleeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	OnMeleeAttackEnd.Broadcast(MeshComp);
}
