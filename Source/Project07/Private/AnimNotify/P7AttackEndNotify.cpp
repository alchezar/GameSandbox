// Copyright (C) 2023, IKinder

#include "AnimNotify/P7AttackEndNotify.h"

void UP7AttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnAnimEnd.Broadcast(MeshComp);
}
