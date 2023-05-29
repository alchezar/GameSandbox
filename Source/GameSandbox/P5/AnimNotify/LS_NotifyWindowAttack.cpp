// Copyright (C) 2023, IKinder

#include "LS_NotifyWindowAttack.h"

ULS_NotifyWindowAttack::ULS_NotifyWindowAttack()
{
	NotifyName = "Attack";
}

void ULS_NotifyWindowAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	LSOnAttackBegin.Broadcast(MeshComp);
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void ULS_NotifyWindowAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	LSOnAttackEnd.Broadcast(MeshComp);
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
